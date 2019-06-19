import React, { Component } from "react";

import Moment from "react-moment";

import { Avatar } from "@rmwc/avatar";
import { Typography } from "@rmwc/typography";
import { Elevation } from "@rmwc/elevation";

import "./message_bubble.scss";

class MessageBubble extends Component {
	render() {
		return (
			<div className="quesync-message-bubble">
				<Elevation z={7} className="quesync-text-bubble">
					<div className="quesync-text-sender-info">
						<Avatar
							className="quesync-text-avatar"
							src={this.props.avatarUrl}
						/>
						<div>
							<Typography use="subtitle1" style={{ fontSize: "0.86rem" }}>
								{this.props.sender}
							</Typography>
						</div>
					</div>
					<Typography className="quesync-text-content" use="body2">
						{this.props.messages.map(message => (
							<div>{message.content}</div>
						))}
					</Typography>
				</Elevation>
				<div className="quesync-text-message-date">
					<Typography use="caption" style={{ fontSize: "0.68rem" }}>
						<Moment unix calendar interval={10000} date={this.props.messages[0].sentAt} />
					</Typography>
				</div>
			</div>
		);
	}
}

export default MessageBubble;