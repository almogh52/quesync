import React, { Component } from "react";
import { connect } from "react-redux";

import { Scrollbars } from "react-custom-scrollbars";

import { TransitionGroup } from "react-transition-group";
import FadeTransition from "./fade_transition";

import MessageBubble from "./message_bubble";
import MessageField from "./message_field";
import Seperator from "./seperator";

import "./channel.scss";
import CallDetails from "./call_details";

class Channel extends Component {
	endDummy = React.createRef();

	constructor(props) {
		super(props);

		this.state = {
			prevChannelId: props.channelId
		};
	}

	componentDidMount = () => {
		this.scrollToBottom(false);
	};

	componentDidUpdate = () => {
		this.scrollToBottom(this.state.prevChannelId === this.props.channelId);

		// If the channel has changed, reset the prev channel id
		if (this.state.prevChannelId !== this.props.channelId) {
			this.setState({
				prevChannelId: this.props.channelId
			});
		}
	};

	getChannelContent = () => {
		const messages = this.props.messages[this.props.channelId];
		const calls = this.props.calls[this.props.channelId];

		const content = messages.concat(calls);

		let currentSender,
			lastDate,
			currentIdx = 0,
			groupedContent = [];

		// Sort the content
		content.sort((e1, e2) => {
			const date1 = e1.sentAt ? e1.sentAt : e1.startDate;
			const date2 = e2.sentAt ? e2.sentAt : e2.startDate;

			if (date1 < date2) return -1;
			else if (date1 > date2) return 1;
			else return 0;
		});

		// For each content element group it if it's a message
		content.forEach(e => {
			// If not a message
			if (!e.sentAt) {
				currentIdx = groupedContent.push(e);

				currentSender = null;
				lastDate = e.startDate;
			}
			// If the message has the current sender of the last message and is in within an hour from the last message, add it to last group
			else if (
				currentSender != null &&
				currentSender === e.senderId &&
				e.sentAt - lastDate <= 60 * 60
			) {
				groupedContent[currentIdx].push(e);

				// Set the last send date
				lastDate = e.sentAt;
			} else {
				// Create a new group with the message
				currentIdx = groupedContent.push([e]) - 1;

				// Set the new current sender
				currentSender = e.senderId;

				// Set the last send date
				lastDate = e.sentAt;
			}
		});

		return groupedContent;
	};

	getNickname = id => this.props.profiles[id].nickname;
	getProfilePhoto = id => {
		return this.props.profiles[id].photo;
	};

	scrollToBottom = smooth =>
		this.endDummy.current.scrollIntoView({
			behavior: smooth ? "smooth" : "auto"
		});

	render() {
		const content = this.getChannelContent();

		return (
			<div className="quesync-channel">
				<div className="quesync-channel-content-wrapper">
					<Scrollbars
						className="quesync-channel-content"
						renderView={({ style, ...props }) => (
							<div
								{...props}
								style={{ ...style, marginBottom: "0", overflowX: "hidden" }}
							></div>
						)}
					>
						<TransitionGroup className="quesync-channel-content">
							{content.map(e =>
								e instanceof Array ? (
									<FadeTransition
										timeout={200}
										unmountOnExit
										key={"message-" + e[0].id}
									>
										<MessageBubble
											key={e[0].id}
											sender={this.getNickname(e[0].senderId)}
											avatar={this.getProfilePhoto(e[0].senderId)}
											messages={e}
											style={{
												paddingTop: "0.8rem"
											}}
										/>
									</FadeTransition>
								) : (
									<FadeTransition
										timeout={200}
										unmountOnExit
										key={"call-" + e.id}
									>
										<CallDetails
											missedCall={!e.joined}
											callerName={
												e.callerId === this.props.user.id
													? "You"
													: this.getNickname(e.callerId)
											}
											startDate={e.startDate}
											endDate={e.endDate}
										/>
									</FadeTransition>
								)
							)}
						</TransitionGroup>
						<div
							className="quesync-channel-content-end-dummy"
							ref={this.endDummy}
						/>
					</Scrollbars>
				</div>
				<Seperator />
				<MessageField
					channelId={this.props.channelId}
					sendCallback={() => this.scrollToBottom(true)}
				/>
			</div>
		);
	}
}

export default connect(state => ({
	user: state.user.user,
	calls: state.voice.calls,
	messages: state.messages.messages,
	profiles: state.users.profiles
}))(Channel);
