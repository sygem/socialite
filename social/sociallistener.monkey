#If TARGET="android"

Import "native/sociallistener.android.java"

#ElseIf TARGET="ios"

Import "native/sociallistener.ios.cpp"

#End

Import sharer

Class LogInOutListener Extends SocialLogInOutListener
    
    Const ERROR:= 0
    Const CANCELLED:= 1

End

Class PostListener Extends SocialPostListener

    Const ERROR:= 0
    Const NO_ACCOUNT:= 1
    Const ACCESS_DENIED:= 2
    Const SENDING_FAILED:= 3
    Const CANCELLED:= 4

End

Class InitializationListener Extends SocialInitializationListener
    
    Const ERROR:= 0

End

Extern

Class SocialLogInOutListener Abstract

    Method OnLoginComplete:Void()
    Method OnLogoutComplete:Void()
    Method OnLoginFailure:Void(errorCode:Int, message:String)
    
End

Class SocialPostListener Abstract

    Method OnPostComplete:Void()
    Method OnPostFailure:Void(errorCode:Int, message:String)
    
End

Class SocialInitializationListener Abstract

    Method OnInitializeComplete:Void()
    Method OnInitializeFailure:Void(errorCode:Int, message:String)
    
End