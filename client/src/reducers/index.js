import { combineReducers } from "redux";

import user from "./userReducer";
import client from "./clientReducer";
import users from "./usersReducer";
import items from "./itemsReducer";
import badges from "./badgesReducer";
import channels from "./channelsReducer";
import messages from "./messagesReducer";
import voice from "./voiceReducer";
import files from "./filesReducer";

export default combineReducers({
	user,
	client,
	users,
	channels,
	messages,
	voice,
	files,
	ui: combineReducers({
		items,
		badges
	})
});
