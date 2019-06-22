export default function reducer(
	state = {
		authenticating: false,
		user: null,
		authError: 0
	},
	action
) {
	switch (action.type) {
		case "USER_REGISTER_PENDING":
		case "USER_LOGIN_PENDING":
			return { ...state, authError: 0 };

		case "USER_REGISTER_REJECTED":
		case "USER_LOGIN_REJECTED":
			return { ...state, authError: action.payload.error };

		case "USER_REGISTER_FULFILLED":
		case "USER_LOGIN_FULFILLED":
		case "USER_SET":
			return { ...state, user: action.payload.user };

		case "AUTH_STARTED":
			return { ...state, authenticating: true };

		case "AUTH_FINISHED":
			return { ...state, authenticating: false };

		case "APPROVE_FRIEND_REQUEST_FULFILLED": {
			const friendId = action.payload.friendId;

			let newFriendRequests = [...state.user.friendRequests];
			let newFriends = [...state.user.friends];

			// Remove from the friend requests the approved friend request
			newFriendRequests = newFriendRequests.filter(friendRequest => {
				return friendRequest.friendId !== friendId;
			})

			// Add to the friends list the new friend
			newFriends.push(friendId);

			return { ...state, user: { ...state.user, friendRequests: newFriendRequests, friends: newFriends } };
		}

		case "REJECT_FRIEND_REQUEST_FULFILLED": {
			const friendId = action.payload.friendId;

			let newFriendRequests = [...state.user.friendRequests];

			// Remove from the friend requests the approved friend request
			newFriendRequests = newFriendRequests.filter(friendRequest => {
				return friendRequest.friendId !== friendId;
			})

			return { ...state, user: { ...state.user, friendRequests: newFriendRequests } };
		}

		default:
			return state;
	}
}
