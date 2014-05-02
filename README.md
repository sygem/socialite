socialite
=========

Social Network Module for MonkeyX

###Platforms

Socialite currently targets Android and iOS

###Networks
- Twitter
- Facebook
- Email

###Examples
Twitter:

        Local t := New TwitterSharer()
        t.ShareText("Tweet tweet!")
        
Email:

        Local t := New EmailSharer()
        t.SetSubject("This is an subject")
        t.SetTitle("Sending emails from MonkeyX")
        t.ShareText("The body of your email goes here...");
        
Facebook:

        Local fb := New FacebookSharer()
        fb.AddPermission(Permission.USER_PHOTOS)
        fb.AddPermission(Permission.EMAIL)
        fb.AddPermission(Permission.PUBLISH_ACTION)
        fb.SetPostListener(New FacebookPostListener())
        fb.SetLoginListener(New FacebookLoginListener(Self))
        fb.Initialize(New FacebookInitializationListener(Self))
        fb.Login()
        fb.ShareText("MonkeyX is cool, thanks to Socialite!");
        
