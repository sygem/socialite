socialite - Facebook
====================

Getting Facebook working is a little more difficult that the other platforms...

Android
-------

- Download the latest Facebook SDK (I used 3.8.0)
- Extract this to the same drive as your Monkey project
- Compile the Monkey project, then edit the project.properties file to point to the facebook library e.g.
      android.library.reference.1=../../../../SDKs/facebook-android-sdk-3.8.0/facebook
- Next time you compile your Monkey project it should build the Facebook SDK as well

This link should help get you started: https://developers.facebook.com/docs/android/getting-started


iOS
---

- Download and extract the latest Facebook SDK
- Compile the Monkey project, then add the Facebook framework from inside XCode

This link should help get you started: https://developers.facebook.com/docs/ios/getting-started

Final Steps
-----------

In your main Monkey file, add the following definitions:

    #FACEBOOK = true
    #FACEBOOK_APPLICATION_ID = "<YOUR APP ID>"
    #FACEBOOK_APPLICATION_NAMESPACE = "<YOUR APP NAMESPACE>"

