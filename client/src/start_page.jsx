import React, { Component } from "react";
import { connect } from "react-redux";

import LoginForm from "./components/login_form";
import RegisterForm from "./components/register_form";
import BackgroundParticles from "./components/background_particles";

import { ThemeProvider } from "@rmwc/theme";
import { Typography } from "@rmwc/typography";
import { Elevation } from "@rmwc/elevation";
import { CircularProgress } from "@rmwc/circular-progress";
import anime from "animejs/lib/anime.es.js";

import {
	sessionAuth,
	setUser,
	logout,
	resetAuthError
} from "./actions/userActions";

import updater from "./updater";

import "@rmwc/circular-progress/circular-progress.css";
import "./start_page.scss";

const electron = window.require("electron");

class StartPage extends Component {
	state = {
		signInVisible: true,
		registerVisible: false
	};

	async componentDidMount() {
		// Set the app as loading
		this.startLoadingAnimation(null, LoginForm, false);

		// If the user wants to logout
		if (this.props.logout) {
			// Wait for the transition to finish and then logout
			setTimeout(() => {
				// Remove the session id
				localStorage.removeItem("_qpsid");

				// Logout
				this.props
					.dispatch(logout())
					.then(() =>
						// Stop the loading animation
						this.stopLoadingAnimation(null, LoginForm)
					)
					.catch(() =>
						// Stop the loading animation
						this.stopLoadingAnimation(null, LoginForm)
					);
			}, 1000);
		} else {
			// Get the Server's IP
			const serverIP = electron.remote.getGlobal("serverIP");

			// Try to conenct to the server
			this.connectRepeat(this.props.client, serverIP, async () => {
				var user = null;
				var sessionId = localStorage.getItem("_qpsid");

				// If the client is already authenticated, continue to the application
				if ((user = this.props.client.auth().getUser())) {
					// Set user
					this.props.dispatch(setUser(user));

					// Update user's data
					await updater.update();

					// Transition to app
					this.transitionToApp();
				} else if (sessionId && sessionId.length) {
					// Try to connect via the session
					await this.props
						.dispatch(sessionAuth(sessionId))
						.then(async () => {
							// Update user's data
							await updater.update();

							// Transition to app
							this.transitionToApp();
						})
						.catch(() => {
							// Remove the invalid session id
							localStorage.removeItem("_qpsid");

							// Stop the loading animation
							this.stopLoadingAnimation(null, LoginForm);
						});
				} else {
					// Stop the loading animation when connection is successful
					this.stopLoadingAnimation(null, LoginForm);
				}
			});
		}
	}

	connectRepeat = async (client, ip, callback) => {
		try {
			// Try to connect to server
			await client.connect(ip);

			// If successful, call the callback function
			callback();
		} catch (ex) {
			// Retry in 500 milliseconds
			setTimeout(() => this.connectRepeat(client, ip, callback), 500);
		}
	};

	startLoadingAnimation = (completeCallback, form, animated = true) => {
		// Create an animation timeline for the title transition + loading animation
		var timeline = anime.timeline({
			duration: animated ? 1300 : 0,
			easing: "easeInOutCirc",
			delay: 250,
			complete: completeCallback
		});

		// Add animation for the title to fill the menu
		timeline.add({
			targets: ".quesync-title-moving",
			width: form.width * 2 + "rem"
		});

		// Reset the title position to make space for the loading indicator
		timeline.add(
			{
				targets: ".quesync-title-text",
				marginTop: "0px"
			},
			900
		);

		// Fade in the loading indicator
		timeline.add(
			{
				targets: ".quesync-loading",
				opacity: "1"
			},
			900
		);
	};

	stopLoadingAnimation = (completeCallback, form) => {
		// Create a timeline for the return of the title animation
		var timeline = anime.timeline({
			duration: 800,
			easing: "easeInOutCirc",
			delay: 250,
			complete: completeCallback
		});

		// Animate the quesync title moving part to return to it's place
		timeline.add({
			targets: ".quesync-title-moving",
			width: form.width + "rem"
		});

		// Fade out the loading indicator
		timeline.add(
			{
				targets: ".quesync-loading",
				opacity: "0"
			},
			0
		);

		// Return the title text to the center
		timeline.add(
			{
				targets: ".quesync-title-text",
				marginTop: "62px"
			},
			0
		);
	};

	startTransition = (currentForm, targetForm) => {
		// Reset the authentication error
		this.props.dispatch(resetAuthError());

		// Create a timeline animation for the transition for the register form
		var timeline = anime.timeline({
			duration: 800,
			easing: "easeInOutCirc",
			delay: 250
		});

		// Make a fade out animation for the login form
		timeline.add(
			{
				targets: "." + currentForm.formClass,
				opacity: 0
			},
			0
		);

		// Make a fade in animation for the register form
		timeline.add(
			{
				targets: "." + targetForm.formClass,
				opacity: 1
			},
			0
		);

		// Make the form holder height bigger
		timeline.add(
			{
				targets: ".quesync-form-holder",
				height: targetForm.height + "rem"
			},
			0
		);
	};

	transitionToApp = () => {
		// Create a timeline for the transition
		var timeline = anime.timeline({
			duration: 1400,
			easing: "easeInOutCirc",
			delay: 250,
			complete: () => {
				// Animate to the app
				this.props.transitionToApp();
			}
		});

		// Animate the quesync title moving part to return to it's place
		timeline.add(
			{
				targets: this.state.signInVisible
					? ".quesync-login-form-transition"
					: ".quesync-register-form-transition",
				width: "100vw",
				height: "100vh"
			},
			400
		);

		// Fade out the quesync title
		timeline.add(
			{
				targets: ".quesync-transition-title",
				opacity: "0"
			},
			400
		);

		// Fade out the loading indicator
		timeline.add(
			{
				targets: ".quesync-loading",
				opacity: "0",
				duration: 400
			},
			0
		);

		// Return the title text to the center
		timeline.add(
			{
				targets: ".quesync-title-text",
				marginTop: "62px",
				duration: 400,
				complete: () => {
					// Set transition opacity
					this.refs.transition.style.opacity = 1;
				}
			},
			0
		);
	};

	transitionToRegister = () => {
		// Disable interaction with the sign in menu and enable interaction with the register menu
		this.setState({
			signInVisible: false,
			registerVisible: true
		});

		// Transition to the register form
		this.startTransition(LoginForm, RegisterForm);
	};

	transitionToLogin = () => {
		// Disable interaction with the register menu and enable interaction with the sign in menu
		this.setState({
			signInVisible: true,
			registerVisible: false
		});

		// Transition to the login form
		this.startTransition(RegisterForm, LoginForm);
	};

	render() {
		return (
			<ThemeProvider
				className="quesync-start-page"
				options={{ primary: "#007EA7", secondary: "#e0e0e0" }}
				style={{ position: "relative", top: 0, left: 0 }}
			>
				<BackgroundParticles
					style={{
						position: "absolute",
						top: "0",
						left: "0",
						minWidth: "100%",
						minHeight: "100%"
					}}
				/>
				<div className="quesync-transition-holder">
					<div
						className={
							"quesync-transition quesync-title " +
							(this.state.signInVisible
								? "quesync-login-form-transition"
								: "quesync-register-form-transition")
						}
						ref="transition"
					>
						<Typography
							className="quesync-transition-title"
							use="headline2"
							style={{ color: "white", userSelect: "none", opacity: "1" }}
						>
							Quesync
						</Typography>
					</div>
				</div>
				<Elevation
					className="quesync-start-menu"
					z="8"
					style={{ pointerEvents: this.props.authenticating ? "none" : "" }}
				>
					<div className="quesync-form-side quesync-title" />
					<div className="quesync-form-side quesync-title quesync-title-moving">
						<Typography
							className="quesync-title-text"
							use="headline2"
							style={{ color: "white", userSelect: "none", marginTop: "55px" }}
						>
							Quesync
						</Typography>
						<CircularProgress
							className="quesync-loading"
							theme="secondary"
							style={{ marginTop: "38px", opacity: "0" }}
						/>
					</div>
					<div
						className="quesync-form-side quesync-form-holder"
						style={{
							width: LoginForm.width + "rem",
							height: LoginForm.height + "rem"
						}}
					>
						<LoginForm
							startLoadingAnimation={this.startLoadingAnimation}
							stopLoadingAnimation={this.stopLoadingAnimation}
							transitionToRegister={this.transitionToRegister}
							transitionToApp={this.transitionToApp}
							interactable={this.state.signInVisible}
						/>
						<RegisterForm
							startLoadingAnimation={this.startLoadingAnimation}
							stopLoadingAnimation={this.stopLoadingAnimation}
							transitionToLogin={this.transitionToLogin}
							transitionToApp={this.transitionToApp}
							interactable={this.state.registerVisible}
						/>
					</div>
				</Elevation>
			</ThemeProvider>
		);
	}
}

export default connect(state => ({
	client: state.client.client,
	authenticating: state.user.authenticating
}))(StartPage);
