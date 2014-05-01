Import socialite

#If TARGET<>"android" And TARGET<>"ios"
'#Error "The email module is only available on the android and ios targets"
#End

#If TARGET="android"

Import "native/facebookSharer.android.java"
#LIBS+="${CD}/native/simplefacebook.jar"
#ANDROID_MANIFEST_APPLICATION+="<activity android:name=~qcom.facebook.LoginActivity~q android:theme=~q@android:style/Theme.Translucent.NoTitleBar~q></activity>"

#ElseIf TARGET="ios"

Import "native/facebookSharer.ios.cpp"
Import "native/ios/FacebookAppDelegate.ios.h"
Import "native/ios/FacebookAppDelegate.ios.cpp"

#End

Interface IFacebookInitializationListener
    Method InitializationComplete:Void()
End

Class FacebookSharer Extends NativeFacebookSharer Implements ISharer

    Method New()
    End
    
    Method Initialize:Void(listener:InitializationListener = Null)
        If listener <> Null
            SetINITListener(listener)
        EndIf
        Init()
    End
    
    Method Login:Void()
        NativeLogin()
    End
    
    Method IsLoggedIn:Bool()
        Return NativeIsLoggedIn()
    End
    
    Method Logout:Void()
        NativeLogout()
    End

    Method ShareText:Void(text:String, name:String = "", caption:String = "", description:String = "", picture:String = "", link:String = "")
        NativeShareText(text, name, caption, description, picture, link)
    End

    Method SetPostListener:Void(listener:PostListener)
        SetPOSTListener(listener)
    End
    
    Method SetLoginListener:Void(listener:LogInOutListener)
        SetLOGINListener(listener)
    End
    
    Method AddPermission:Void(p:Permission)
        NativeAddPermission(p)
    End
End

Class Permission Extends NativePermission

    Global BASIC_INFO:Permission = New Permission("basic_info")
    
    Global USER_ABOUT_ME:Permission = New Permission("user_about_me")
    Global FRIENDS_ABOUT_ME:Permission = New Permission("friends_about_me")

    Global USER_ACTIVITIES:Permission = New Permission("user_activities")
    Global FRIENDS_ACTIVITIES:Permission = New Permission("friends_activities")

    Global USER_BIRTHDAY:Permission = New Permission("user_birthday")
    Global FRIENDS_BIRTHDAY:Permission = New Permission("friends_birthday")

    Global USER_CHECKINS:Permission = New Permission("user_checkins")
    Global FRIENDS_CHECKINS:Permission = New Permission("friends_checkins")

    Global USER_EDUCATION_HISTORY:Permission = New Permission("user_education_history")
    Global FRIENDS_EDUCATION_HISTORY:Permission = New Permission("friends_education_history")

    Global USER_EVENTS:Permission = New Permission("user_events")
    Global FRIENDS_EVENTS:Permission = New Permission("friends_events")

    Global USER_GROUPS:Permission = New Permission("user_groups")
    Global FRIENDS_GROUPS:Permission = New Permission("friends_groups")

    Global USER_HOMETOWN:Permission = New Permission("user_hometown")
    Global FRIENDS_HOMETOWN:Permission = New Permission("friends_hometown")

    Global USER_INTERESTS:Permission = New Permission("user_interests")
    Global FRIENDS_INTERESTS:Permission = New Permission("friends_interests")

    Global USER_PHOTOS:Permission = New Permission("user_photos")
    Global FRIENDS_PHOTOS:Permission = New Permission("frienphotos")

    Global USER_LIKES:Permission = New Permission("user_activities")
    Global FRIENDS_LIKES:Permission = New Permission("friends_activities")

    Global USER_NOTES:Permission = New Permission("user_notes")
    Global FRIENDS_NOTES:Permission = New Permission("friends_notes")

    Global USER_ONLINE_PRESENCE:Permission = New Permission("user_online_presence")
    Global FRIENDS_ONLINE_PRESENCE:Permission = New Permission("friends_online_presence")

    Global USER_RELIGION_POLITICS:Permission = New Permission("user_religion_politics")
    Global FRIENDS_RELIGION_POLITICS:Permission = New Permission("friends_religion_politics")

    Global USER_RELATIONSHIPS:Permission = New Permission("user_relationships")
    Global FRIENDS_RELATIONSHIPS:Permission = New Permission("friends_relationships")

    Global USER_RELATIONSHIP_DETAILS:Permission = New Permission("user_relationship_details")
    Global FRIENDS_RELATIONSHIP_DETAILS:Permission = New Permission("friends_relationship_details")

    Global USER_STATUS:Permission = New Permission("user_status")
    Global FRIENDS_STATUS:Permission = New Permission("friends_status")

    Global USER_SUBSCRIPTIONS:Permission = New Permission("user_subscriptions")
    Global FRIENDS_SUBSCRIPTIONS:Permission = New Permission("friends_subscriptions")

    Global USER_VIDEOS:Permission = New Permission("user_videos")
    Global FRIENDS_VIDEOS:Permission = New Permission("friends_videos")

    Global USER_WEBSITE:Permission = New Permission("user_website")
    Global FRIENDS_WEBSITE:Permission = New Permission("friends_website")

    Global USER_WORK_HISTORY:Permission = New Permission("user_work_history")
    Global FRIENDS_WORK_HISTORY:Permission = New Permission("friends_work_history")

    Global USER_LOCATION:Permission = New Permission("user_location")
    Global FRIENDS_LOCATION:Permission = New Permission("friends_location")

    Global USER_PHOTO_VIDEO_TAGS:Permission = New Permission("user_photo_video_tags")
    Global FRIENDS_PHOTO_VIDEO_TAGS:Permission = New Permission("friends_photo_video_tags")

    Global READ_FRIENDLISTS:Permission = New Permission("read_friendlists")
    Global READ_MAILBOX:Permission = New Permission("read_mailbox")
    Global READ_REQUESTS:Permission = New Permission("read_requests")
    Global READ_STREAM:Permission = New Permission("read_stream")
    Global READ_INSIGHTS:Permission = New Permission("read_insights")
    Global XMPP_LOGIN:Permission = New Permission("xmpp_login")
    Global EMAIL:Permission = New Permission("email")
    
    Global PUBLISH_ACTION:Permission = New Permission("publish_actions", Type.PUBLISH)
    Global PUBLISH_STREAM:Permission = New Permission("publish_stream", Type.PUBLISH)
    
    Global ADS_MANAGEMENT:Permission = New Permission("ads_management", Type.PUBLISH)
    Global CREATE_EVENT:Permission = New Permission("create_event", Type.PUBLISH)
    Global RSVP_EVENT:Permission = New Permission("rsvp_event", Type.PUBLISH)
    Global MANAGE_FRIENDLIST:Permission = New Permission("manage_friendlists", Type.PUBLISH)
    Global MANAGE_NOTIFICATIONS:Permission = New Permission("manage_notifications", Type.PUBLISH)
    Global MANAGE_PAGES:Permission = New Permission("manage_pages", Type.PUBLISH)
    
    Private
    Method New(value:String, type:Type = Type.READ)
        SetValue(value)
        SetType(type)
    End
End

Class Type Extends NativeType

    Global READ:Type = New Type("READ")
    Global PUBLISH:Type = New Type("PUBLISH")

    Private
    Method New(t:String)
        Init(t)
    End
    'Field 
End

Private

#If TARGET<>"android" And TARGET<>"ios"

Class NativeFacebookSharer

    Private
    Method NativeShareText:Void(text:String, name:String = "", caption:String = "", description:String = "", picture:String = "", link:String = "")
        Print("Share Text: " + text)
    End
    Method Init:Void()
    End
    Method SetPOSTListener:Void(listener:PostListener)
    End
    Method SetLOGINListener:Void(listener:LogInOutListener)
    End
    Method SetINITListener:Void(listener:InitializationListener)
    End
    Method NativeLogin:Void()
    End
    Method NativeIsLoggedIn:Bool()
    End
    Method NativeLogout:Void()
    End
    Method NativeAddPermission:Void(permission:Permission)
    End
End
Class NativePermission
    Method SetValue:Void(value:String)
    End
    Method SetType:Void(type:Type)
    End
End
Class NativeType
    Method Init:Void(t:String)
    End
End
#Else

Extern

Class NativeFacebookSharer

    Private
    Method NativeShareText:Void(text:String, name:String = "", caption:String = "", description:String = "", picture:String = "", link:String = "")
    Method NativeLogin:Void()
    Method NativeIsLoggedIn:Bool()
    Method NativeLogout:Void()
    Method Init:Void()
    Method SetPOSTListener:Void(listener:PostListener)
    Method SetLOGINListener:Void(listener:LogInOutListener)
    Method SetINITListener:Void(listener:InitializationListener)
    Method NativeAddPermission:Void(permission:Permission)

End
Class NativePermission
    Method SetValue:Void(value:String)
    Method SetType:Void(type:Type)
End
Class NativeType
    Method Init:Void(t:String)
End

#End