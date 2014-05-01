
#if TWITTER or FACEBOOK or EMAIL

Import social.sociallistener
Import social.sharer

#if TWITTER
Import twitter
#endif
#if FACEBOOK
Import facebook
#endif
#if EMAIL
Import email
#endif

#if TARGET="android"
Import "native/socialite.android.java"
#endif

Extern

Class SocialNetwork
    
    Global TWITTER:SocialNetwork

End

#endif