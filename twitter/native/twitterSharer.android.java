import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.util.Log;
import android.webkit.WebView;
import android.webkit.WebViewClient;

import twitter4j.Twitter;
import twitter4j.TwitterException;
import twitter4j.TwitterFactory;
import twitter4j.User;
import twitter4j.auth.AccessToken;
import twitter4j.auth.RequestToken;
import twitter4j.conf.Configuration;
import twitter4j.conf.ConfigurationBuilder;

import com.sygem.monkey.socialite.twitter.*;

class NativeTwitterSharer {

    private SocialPostListener listener;
    private SharedPreferences prefs;
    private Handler mTwitterHandler;
    private TwitterUtils utils = new TwitterUtils();
    private static RequestToken requestToken;
    
    private String error;

    final Runnable mUpdateTwitterNotification = new Runnable() {
        public void run() {
            if (listener != null) {
                listener.OnPostComplete();
            }
        }
    };
    final Runnable mUpdateTwitterFailNotification = new Runnable() {
        public void run() {
            if (listener != null) {
                listener.OnPostFailure(0, error);
            }
        }
    };

    public void Init() {
        prefs = BBAndroidGame.AndroidGame().GetActivity().getSharedPreferences(TwitterConstants.PREFERENCE_NAME, Context.MODE_PRIVATE);
        BBAndroidGame.AndroidGame().GetActivity().runOnUiThread(new Runnable() {
            public void run() {
                mTwitterHandler = new Handler();
            }
        });
    }
    
    public void SetListener(SocialPostListener listener) {
        this.listener = listener;
    }
    
    public void NativeShareText(String text) {
    	if (utils.isAuthenticated(prefs)) {
    		tweet(text);
    	} else {
            utils.loginToTwitter(prefs, text);
    	}
    }

	private void tweet(final String text) {
		Thread t = new Thread() {
	        public void run() {

	        	try {
	        		utils.sendTweet(prefs,text);
	        		mTwitterHandler.post(mUpdateTwitterNotification);
				} catch (Exception ex) {
					ex.printStackTrace();
                    error = ex.getMessage();
	        		mTwitterHandler.post(mUpdateTwitterFailNotification);
				}
	        }

	    };
	    t.start();
	}

    class TwitterUtils extends ActivityDelegate {
    
        // Twitter oauth urls
        static final String URL_TWITTER_AUTH = "auth_url";
        static final String URL_TWITTER_OAUTH_VERIFIER = "oauth_verifier";
        static final String URL_TWITTER_OAUTH_TOKEN = "oauth_token";
    
    	public boolean isAuthenticated(SharedPreferences prefs) {
    
    		String token = prefs.getString(TwitterConstants.PREF_KEY_TOKEN, "");
    		String secret = prefs.getString(TwitterConstants.PREF_KEY_SECRET, "");
    
    		AccessToken accessToken = new AccessToken(token,secret);
    
            ConfigurationBuilder builder = new ConfigurationBuilder();
            builder.setOAuthConsumerKey(MonkeyConfig.TWITTER_CONSUMER_KEY);
            builder.setOAuthConsumerSecret(MonkeyConfig.TWITTER_CONSUMER_SECRET);
    
            Twitter twitter = new TwitterFactory(builder.build()).getInstance(accessToken);
    
    		try {
    			twitter.getAccountSettings();
    			return true;
    		} catch (TwitterException e) {
    			return false;
    		}
    	}
    
    	public void sendTweet(SharedPreferences prefs, String msg) throws Exception {
    		String token = prefs.getString(TwitterConstants.PREF_KEY_TOKEN, "");
    		String secret = prefs.getString(TwitterConstants.PREF_KEY_SECRET, "");
    
    		AccessToken accessToken = new AccessToken(token,secret);
    
            ConfigurationBuilder builder = new ConfigurationBuilder();
            builder.setOAuthConsumerKey(MonkeyConfig.TWITTER_CONSUMER_KEY);
            builder.setOAuthConsumerSecret(MonkeyConfig.TWITTER_CONSUMER_SECRET);
    
            Twitter twitter = new TwitterFactory(builder.build()).getInstance(accessToken);
    
            twitter.updateStatus(msg);
    	}	
    
        /**
         * Function to login twitter
         * */
        void loginToTwitter(SharedPreferences prefs, String text) {
            if (!TwitterLoginActivity.isConnected(BBAndroidGame.AndroidGame().GetActivity())) {

                BBAndroidGame.AndroidGame().AddActivityDelegate(this);

                Intent twitterLoginIntent = new Intent(BBAndroidGame.AndroidGame().GetActivity(), TwitterLoginActivity.class);
                twitterLoginIntent.putExtra(TwitterLoginActivity.TWITTER_CONSUMER_KEY, MonkeyConfig.TWITTER_CONSUMER_KEY);
                twitterLoginIntent.putExtra(TwitterLoginActivity.TWITTER_CONSUMER_SECRET, MonkeyConfig.TWITTER_CONSUMER_SECRET);
                twitterLoginIntent.putExtra(TwitterLoginActivity.TWITTER_CALLBACK_URL, MonkeyConfig.TWITTER_CALLBACK_URL);
                twitterLoginIntent.putExtra("tweet_msg",text);

                BBAndroidGame.AndroidGame().GetActivity().startActivityForResult(twitterLoginIntent, TWITTER_LOGIN_REQUEST_CODE);
            }
        }
    
        private static final int TWITTER_LOGIN_REQUEST_CODE = 1;
    
    	public void onActivityResult( int requestCode,int resultCode,Intent data) {
            BBAndroidGame.AndroidGame().RemoveActivityDelegate(this);
            super.onActivityResult(requestCode, resultCode, data);

            if (requestCode == TWITTER_LOGIN_REQUEST_CODE) {
                if (resultCode == TwitterLoginActivity.TWITTER_LOGIN_RESULT_CODE_SUCCESS) {
                    tweet(data.getStringExtra("tweet_msg"));
                } else if (resultCode == TwitterLoginActivity.TWITTER_LOGIN_RESULT_CODE_FAILURE) {
                    error = "Twitter Login Failure";
	        		mTwitterHandler.post(mUpdateTwitterFailNotification);
                } else {
                //
                }
            }
    	}
    
    }
    
}
