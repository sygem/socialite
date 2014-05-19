Import socialite

#If TARGET<>"android" And TARGET<>"ios"
'#Error "The Twitter module is only available on the android and ios targets"
#End

#if not TWITTER_CALLBACK_URL
#TWITTER_CALLBACK_URL = "x-oauthflow-twitter://twitterlogin"
#endif

#If TARGET="android"

Import "native/twitterSharer.android.java"
#LIBS+="${CD}/native/twitter4j-core-4.0.1.jar"
#LIBS+="${CD}/native/socialitetwitter.jar"
#ANDROID_MANIFEST_APPLICATION+="<activity android:name=~qcom.sygem.monkey.socialite.twitter.TwitterLoginActivity~q> <intent-filter><action android:name=~qandroid.intent.action.VIEW~q/><category android:name=~qandroid.intent.category.DEFAULT~q/><category android:name=~qandroid.intent.category.BROWSABLE~q/><data android:scheme=~qx-oauthflow-twitter~q android:host=~qtwitterlogin~q/></intent-filter> </activity>"

#ElseIf TARGET="ios"

Import "native/twitterSharer.ios.cpp"

#LIBS+="Social.framework"
#LIBS+="Accounts.framework"

#End

#TWITTER_CONSUMER_KEY=""
#TWITTER_CONSUMER_SECRET=""

Class TwitterSharer Extends NativeTwitterSharer Implements ISharer

    Method New()
        Init()
    End

    Method ShareText:Void(text:String)
        NativeShareText(text)
    End

    Method SetPostListener:Void(listener:PostListener)
        SetListener(listener)
    End
End

Private

#If TARGET<>"android" And TARGET<>"ios"

Class NativeTwitterSharer

    Private
    Method NativeShareText:Void(text:String)
        Print("Share Text: " + text)
    End
    Method Init:Void()
    End
    Method SetListener:Void(listener:PostListener)
    End
End

#Else

Extern

Class NativeTwitterSharer
    Private
    Method NativeShareText:Void(text:String)
    Method Init:Void()
    Method SetListener:Void(listener:PostListener)

End

#End