import React, { Component } from "react";
import { connect } from "react-redux";

import DrawerPage from "../components/page_layouts/drawer_page";

import { ListItem, ListItemGraphic, ListItemText } from "@rmwc/list";
import { Avatar } from "@rmwc/avatar";

import "./friends_page.scss";

class FriendsPage extends Component {
	render() {
		const friends = this.props.user.friends.map(
			friendId => this.props.profiles[friendId].nickname
		);

		return (
			<DrawerPage
				className="quesync-friends-page"
				drawerContent={friends.map((friend, idx) => (
					<ListItem className="quesync-friend-list-item" key={idx}>
						<ListItemGraphic
							icon={
								<Avatar
									className="quesync-friend-avatar"
									src="https://jamesmfriedman.github.io/rmwc/images/avatars/captainamerica.png"
								/>
							}
						/>
						<ListItemText className="quesync-friend-text">{friend}</ListItemText>
					</ListItem>
				))}
				tableWidth="14rem"
			/>
		);
	}
}

export default connect(state => ({
	user: state.auth.user,
	profiles: state.users.profiles
}))(FriendsPage);
