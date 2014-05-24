import com.sromku.simple.fb.*;
import com.sromku.simple.fb.actions.*;
import com.sromku.simple.fb.entities.*;
import com.sromku.simple.fb.listeners.*;
import com.sromku.simple.fb.utils.*;

import com.facebook.SessionState;

import android.util.Log;
import java.util.ArrayList;

class NativeFacebookSharer extends ActivityDelegate {

    protected static final String TAG = "Socialite.Facebook";

    private SocialPostListener postListener;
    private SocialLogInOutListener loginListener;
    private SocialInitializationListener initListener;
    
	private SimpleFacebook mSimpleFacebook;
    
    private ArrayList<Permission> permissions = new ArrayList<Permission>();
        
    public void Init() {
        BBAndroidGame.AndroidGame().AddActivityDelegate(this);

        Permission[] _permissions = permissions.toArray(new Permission[permissions.size()]);

        SimpleFacebookConfiguration configuration = new SimpleFacebookConfiguration.Builder()
            .setAppId(MonkeyConfig.FACEBOOK_APPLICATION_ID)
            .setNamespace(MonkeyConfig.FACEBOOK_APPLICATION_NAMESPACE)
            .setPermissions(_permissions)
            .build();
            
        SimpleFacebook.setConfiguration(configuration);
        mSimpleFacebook = SimpleFacebook.getInstance(BBAndroidGame.AndroidGame().GetActivity());
        
        if (mSimpleFacebook.getSession() != null && mSimpleFacebook.getSession().getState().equals(SessionState.CREATED_TOKEN_LOADED)) {
            mSimpleFacebook.login(mOnInitLoginListener);
        } else {
            if (initListener != null) {
                initListener.OnInitializeComplete();
            }
        }
    }
    
    public void SetPOSTListener(SocialPostListener listener) {
        this.postListener = listener;
    }
    
    public void SetLOGINListener(SocialLogInOutListener listener) {
        this.loginListener = listener;
    }
    
    public void SetINITListener(SocialInitializationListener listener) {
        this.initListener = listener;
    }
    
    public void NativeShareText(final String text, final String name, final String caption, final String description, final String picture, final String link) {
        BBAndroidGame.AndroidGame().GetActivity().runOnUiThread(new Runnable() {
            public void run() {
                Feed.Builder builder = new Feed.Builder();
                if (text != null && text.length() > 0) builder.setMessage(text);
                if (name != null && name.length() > 0) builder.setName(name);
                if (caption != null && caption.length() > 0) builder.setCaption(caption);
                if (description != null && description.length() > 0) builder.setDescription(description);
                if (picture != null && picture.length() > 0) builder.setPicture(picture);
                if (link != null && link.length() > 0) builder.setLink(link);
                Feed feed = builder.build();
                mSimpleFacebook.publish(feed, onPublishListener);
            }
        });
    }

    public void NativeLogin() {
        mSimpleFacebook.login(mOnLoginListener);
    }

    public void NativeLogout() {
        mSimpleFacebook.logout(mOnLogoutListener);
    }

    public boolean NativeIsLoggedIn() {
        return mSimpleFacebook.isLogin();
    }
    
    public void NativeAddPermission(NativePermission permission) {
        permissions.add(permission.getPermission());
    }

    // Activity Delegate Methods
	public void onStart() {
	}
	public void onRestart() {
	}
	public void onResume() {
        mSimpleFacebook = SimpleFacebook.getInstance(BBAndroidGame.AndroidGame().GetActivity());
	}
	public void onPause() {
	}
	public void onStop() {
	}
	public void onDestroy() {
	}
	public void onActivityResult(int requestCode,int resultCode,Intent data) {
		mSimpleFacebook.onActivityResult(BBAndroidGame.AndroidGame().GetActivity(), requestCode, resultCode, data);
	}

	// Login listener
	private OnLoginListener mOnLoginListener = new OnLoginListener() {

		@Override
		public void onFail(String reason) {
			Log.w(TAG, "Failed to login: "+reason);
		}

		@Override
		public void onException(Throwable throwable) {
			Log.e(TAG, "Bad thing happened", throwable);
		}

		@Override
		public void onThinking() {
		}

		@Override
		public void onLogin() {
            if (loginListener != null) {
                loginListener.OnLoginComplete();
            }
		}

		@Override
		public void onNotAcceptingPermissions(Permission.Type type) {
		}
	};

	// Initialization Login listener
	private OnLoginListener mOnInitLoginListener = new OnLoginListener() {

		@Override
		public void onFail(String reason) {
			Log.w(TAG, "Failed to login: "+reason);
            if (initListener != null) {
                initListener.OnInitializeFailure(0, reason);
            }
		}

		@Override
		public void onException(Throwable throwable) {
			Log.e(TAG, "Bad thing happened", throwable);
            if (initListener != null) {
                initListener.OnInitializeFailure(0, throwable.getMessage());
            }
		}

		@Override
		public void onThinking() {
		}

		@Override
		public void onLogin() {
            if (initListener != null) {
                initListener.OnInitializeComplete();
            }
            if (loginListener != null) {
                loginListener.OnLoginComplete();
            }
		}

		@Override
		public void onNotAcceptingPermissions(Permission.Type type) {
		}
	};

	// Logout listener
	private OnLogoutListener mOnLogoutListener = new OnLogoutListener() {

		@Override
		public void onFail(String reason) {
			Log.w(TAG, "Failed to logout: "+reason);
            if (loginListener != null) {
                loginListener.OnLoginFailure(0,reason);
            }
		}

		@Override
		public void onException(Throwable throwable) {
			Log.e(TAG, "Bad thing happened", throwable);
		}

		@Override
		public void onThinking() {
		}

		@Override
		public void onLogout() {
            if (loginListener != null) {
                loginListener.OnLogoutComplete();
            }
		}

	};
    
    private OnPublishListener onPublishListener = new OnPublishListener() {
        @Override
        public void onComplete(String postId) {
            Log.i(TAG, "Published successfully. The new post id = " + postId);
            if (postListener != null) {
                postListener.OnPostComplete();
            }
        }
    
        /* 
         * You can override other methods here: 
         * onThinking(), onFail(String reason), onException(Throwable throwable)
         */
    };
}

class NativePermission {

    private Permission permission;

    void SetValue(String value) {
        permission = Permission.fromValue(value);
    }
    
    Permission getPermission() {
        return permission;
    }
    
    void SetType(NativeType type) {
    }
    
}

// Not actually needed on Android, because of Permission.fromValue()
// is it needed on iOS? Maybe not, in which case it can be removed
class NativeType {
    void Init(String t) {
    }
}