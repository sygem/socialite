#import <FacebookSDK/FacebookSDK.h>

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

class NativeType : public Object {
    String type;
public:
    void Init(String type);
    bool isPublish();
    bool isRead();
};

void NativeType::Init(String type) {
    this->type = type;
}
bool NativeType::isPublish() {
    return type == "PUBLISH";
}
bool NativeType::isRead() {
    return type == "READ";
}

class NativePermission : public Object {
    String value;
    NativeType *type;
public:
    void SetValue(String value);
    void SetType(NativeType *type);
    String GetValue();
    NativeType* GetType();
};

void NativePermission::SetValue(String value) {
    this->value = value;
}
void NativePermission::SetType(NativeType *type) {
    this->type = type;
}
String NativePermission::GetValue() {
    return value;
}
NativeType* NativePermission::GetType() {
    return type;
}

class NativeFacebookSharer : public Object {

    SocialPostListener *_postListener;
    SocialLogInOutListener *_loginListener;
    SocialInitializationListener *_initListener;
    NSMutableArray *permissions = [NSMutableArray arrayWithCapacity:5];
    bool loggedIn;    
    
public:
    NativeFacebookSharer() : loggedIn(false) {}
    void NativeShareText(String text, String name, String caption, String description, String picture, String link);
    void Init();
    void SetPOSTListener(SocialPostListener *listener);
    void SetLOGINListener(SocialLogInOutListener *listener);
    void SetINITListener(SocialInitializationListener *listener);
    void NativeLogin();
    bool NativeIsLoggedIn();
    void NativeLogout();
    void NativeAddPermission(NativePermission *permission);
private:
    void sessionStateChanged(FBSession *session, FBSessionState state, NSError *error);
    void makeRequestToShare(String text, String name, String caption, String description, String picture, String link);
    void openSession(bool ui);
};

void NativeFacebookSharer::NativeShareText(String text, String name, String caption, String description, String picture, String link) {
    NSArray *permissionsNeeded = @[@"publish_actions"];
    // Request the permissions the user currently has
    [FBRequestConnection startWithGraphPath:@"/me/permissions"
                        completionHandler:^(FBRequestConnection *connection, id result, NSError *error) {
                          if (!error){
                            NSDictionary *currentPermissions= [(NSArray *)[result data] objectAtIndex:0];
                            NSMutableArray *requestPermissions = [[NSMutableArray alloc] initWithArray:@[]];
                            
                            // Check if all the permissions we need are present in the user's current permissions
                            // If they are not present add them to the permissions to be requested
                            for (NSString *permission in permissionsNeeded){
                              if (![currentPermissions objectForKey:permission]){
                                [requestPermissions addObject:permission];
                              }
                            }
                            
                            // If we have permissions to request
                            if ([requestPermissions count] > 0){
                              // Ask for the missing permissions
                              [FBSession.activeSession requestNewPublishPermissions:requestPermissions
                                                                    defaultAudience:FBSessionDefaultAudienceFriends
                                                                  completionHandler:^(FBSession *session, NSError *error) {
                                                                    if (!error) {
                                                                      // Permission granted, we can request the user information
                                                                      makeRequestToShare(text, name, caption, description, picture, link);
                                                                    } else {
                                                                      // An error occurred, handle the error
                                                                      // See our Handling Errors guide: https://developers.facebook.com/docs/ios/errors/
                                                                      //NSLog(@"Post error: %@", error.description);
                                                                      if (_postListener != Nil) {
                                                                        _postListener->OnPostFailure(0,String([error description]));
                                                                      }
                                                                    }
                                                                  }];
                            } else {
                              // Permissions are present, we can request the user information
                                makeRequestToShare(text, name, caption, description, picture, link);
                            }
                            
                          } else {
                            // There was an error requesting the permission information
                            // See our Handling Errors guide: https://developers.facebook.com/docs/ios/errors/
                            //NSLog(@"Post error: %@", error.description);
                            if (_postListener != Nil) {
                              _postListener->OnPostFailure(0,String([error description]));
                            }
                          }
                        }];
                        
}

void NativeFacebookSharer::makeRequestToShare(String text, String name, String caption, String description, String picture, String link) {
    NSMutableDictionary *params = [NSMutableDictionary dictionary];
    if (text != Nil && text.Length() > 0) params[@"message"] = text.ToNSString(); 
    if (name != Nil && name.Length() > 0) params[@"name"] = name.ToNSString(); 
    if (caption != Nil && caption.Length() > 0) params[@"caption"] = caption.ToNSString(); 
    if (description != Nil && description.Length() > 0) params[@"description"] = description.ToNSString(); 
    if (picture != Nil && picture.Length() > 0) params[@"picture"] = picture.ToNSString(); 
    if (link != Nil && link.Length() > 0) params[@"link"] = link.ToNSString(); 
    [FBRequestConnection startWithGraphPath:@"/me/feed"
                               parameters:params
                               HTTPMethod:@"POST"
                        completionHandler:^(FBRequestConnection *connection, id result, NSError *error) {
                          if (!error) {
                            // Link posted successfully to Facebook
                            //NSLog(@"result: %@", result);
                            if (_postListener != Nil) {
                              _postListener->OnPostComplete();
                            }
                          } else {
                            // An error occurred, we need to handle the error
                            // See: https://developers.facebook.com/docs/ios/errors
                            //NSLog(@"Error: %@", error.description);
                            if (_postListener != Nil) {
                              _postListener->OnPostFailure(0,String([error description]));
                            }
                          }
                        }];
    }

void NativeFacebookSharer::Init() {
    if (FBSession.activeSession.state == FBSessionStateCreatedTokenLoaded) {
        __block int sessionChange = 0;
        [FBSession openActiveSessionWithReadPermissions:permissions
                                            allowLoginUI:false
                                            completionHandler:^(FBSession *session, FBSessionState state, NSError *error) {
                                                // Handler for session state changes
                                                // This method will be called EACH time the session state changes,
                                                // also for intermediate states and NOT just when the session open
                                                if (_initListener != Nil && sessionChange == 0) {
                                                    _initListener->OnInitializeComplete();
                                                }
                                                sessionChange++;
                                                sessionStateChanged(session,state,error);
                                            }];
    } else {
        if (_initListener != Nil) {
            _initListener->OnInitializeComplete();
        }
    }
}

void NativeFacebookSharer::openSession(bool ui) {
    if (NativeIsLoggedIn()) return;
    [FBSession openActiveSessionWithReadPermissions:permissions
                                        allowLoginUI:ui
                                        completionHandler:^(FBSession *session, FBSessionState state, NSError *error) {
                                            // Handler for session state changes
                                            // This method will be called EACH time the session state changes,
                                            // also for intermediate states and NOT just when the session open
                                            sessionStateChanged(session,state,error);
                                        }];
}

void NativeFacebookSharer::SetPOSTListener(SocialPostListener *listener) {
    _postListener = listener;
}

void NativeFacebookSharer::SetLOGINListener(SocialLogInOutListener *listener) {
    _loginListener = listener;
}

void NativeFacebookSharer::SetINITListener(SocialInitializationListener *listener) {
    _initListener = listener;
}

void NativeFacebookSharer::NativeLogin() {
    openSession(YES);
}
bool NativeFacebookSharer::NativeIsLoggedIn() {
    return (FBSession.activeSession.state == FBSessionStateOpen || FBSession.activeSession.state == FBSessionStateCreatedOpening || FBSession.activeSession.state == FBSessionStateOpenTokenExtended);
}
void NativeFacebookSharer::NativeLogout() {
    if (FBSession.activeSession.state == FBSessionStateOpen || FBSession.activeSession.state == FBSessionStateOpenTokenExtended) {
        [FBSession.activeSession closeAndClearTokenInformation];
    }
}
void NativeFacebookSharer::NativeAddPermission(NativePermission *permission) {
    if (permission->GetType()->isRead()) {
        [permissions addObject:permission->GetValue().ToNSString()];
    }
}

void NativeFacebookSharer::sessionStateChanged(FBSession *session, FBSessionState state, NSError *error) {
    if (!error && state == FBSessionStateOpen && !loggedIn) {
        //NSLog(@"Session opened");
        loggedIn = true;
        if (_loginListener != Nil) {
            _loginListener->OnLoginComplete();
        }
        return;
    }
  
    if (state == FBSessionStateClosed || state == FBSessionStateClosedLoginFailed) {
        // If the session is closed
        //NSLog(@"Session closed");
        loggedIn = false;
        if (_loginListener != Nil) {
            _loginListener->OnLogoutComplete();
        }
    }
    
    if (error) {
        if ([FBErrorUtility errorCategoryForError:error] == FBErrorCategoryUserCancelled) {
            if (_loginListener != Nil) {
                _loginListener->OnLoginFailure(1,"User cancelled login");
            }
            //NSLog(@"User cancelled login");
        }
    }
}
