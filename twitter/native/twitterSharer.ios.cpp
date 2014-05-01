#import <Social/Social.h>
#import <Accounts/Accounts.h>


class NativeTwitterSharer : public Object {

    SocialPostListener *_listener;

public:
    void NativeShareText(String text);
    void Init();
    void SetListener(SocialPostListener *listener);
};

void NativeTwitterSharer::NativeShareText(String text) {
    ACAccountStore *accountStore = [[ACAccountStore alloc] init];
    ACAccountType *accountType = [accountStore accountTypeWithAccountTypeIdentifier:ACAccountTypeIdentifierTwitter];

    if ([SLRequest class] != Nil) {
        // iOS 6 or above - use the Social.framework
        [accountStore requestAccessToAccountsWithType:accountType withCompletionHandler:^(BOOL granted, NSError *error) {
            if (granted) {
                // Populate array with all available Twitter accounts
                NSArray *arrayOfAccounts = [accountStore accountsWithAccountType:accountType];
                if ([arrayOfAccounts count] > 0) {
                    //use the first account available
                    ACAccount *acct = [arrayOfAccounts objectAtIndex:0];
        
                    //create this request 
                    SLRequest *postRequest = [SLRequest requestForServiceType:SLServiceTypeTwitter requestMethod:SLRequestMethodPOST URL:[NSURL URLWithString:@"https://api.twitter.com/1.1/statuses/update.json"] parameters:  [NSDictionary dictionaryWithObject:text.ToNSString() forKey:@"status"]];
                    //Post the request
                    [postRequest setAccount:acct];
        
                     //manage the response
                    [postRequest performRequestWithHandler:^(NSData *responseData, NSHTTPURLResponse *urlResponse, NSError *error) {
                        if (error) {
                            //if there is an error while posting the tweet
                            //UIAlertView *alert = [[UIAlertView alloc]initWithTitle:@"Twitter" message:@"Error in posting" delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
                            //[alert show];
                            if (_listener != Nil) {
                                _listener->OnPostFailure(0, String([error localizedDescription]));
                            }
                        } else {
                            // on successful posting the tweet
                            //NSLog(@"Twitter response, HTTP response: %i", [urlResponse statusCode]);
                            //UIAlertView *alert = [[UIAlertView alloc]initWithTitle:@"Twitter" message:@"Successfully posted" delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
                            //[alert show];
                            if (_listener != Nil) {
                                _listener->OnPostComplete();
                            }
                        }
                    }];
                }
            } else {
                if (error != Nil) {
                    // if there is an error, there is no system twitter account
                    //UIAlertView *alert = [[UIAlertView alloc]initWithTitle:@"Twitter" message:@"No system Twitter account" delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
                    //[alert show];
                    if (_listener != Nil) {
                        _listener->OnPostFailure(1, "No system Twitter account");
                    }
                } else {
                    // if there is no error, there is a system account but the user has denied access to your app
                    //UIAlertView *alert = [[UIAlertView alloc]initWithTitle:@"Twitter" message:@"Twitter has denied access to this app" delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
                    //[alert show];
                    if (_listener != Nil) {
                        _listener->OnPostFailure(2, "Twitter has denied access to this app");
                    }
                }
            }
        }];
    } else {
        // TODO: Handle this error condition
    }
}

void NativeTwitterSharer::Init() {
}

void NativeTwitterSharer::SetListener(SocialPostListener *listener) {
    _listener = listener;
}

