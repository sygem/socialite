#import <MessageUI/MessageUI.h>

@interface SocialiteEmailDelegate : NSObject <MFMailComposeViewControllerDelegate> {

    SocialNetworkListener *listener;
}

+(SocialiteEmailDelegate*)delegateWithListener:(SocialNetworkListener*)listener;

@end

@implementation SocialiteEmailDelegate

+(SocialiteEmailDelegate*)delegateWithListener:(SocialNetworkListener*)_listener {
    SocialiteEmailDelegate *delegate = [[SocialiteEmailDelegate alloc] init];
    delegate->listener = _listener;
    return delegate;
}

-(void)mailComposeController:(MFMailComposeViewController *)controller didFinishWithResult:(MFMailComposeResult)result error:(NSError *)error {
    switch (result) {
        case MFMailComposeResultCancelled:
            //NSLog(@"Mail cancelled");
            if (listener != Nil) {
                listener->OnPostFailure(4, "Cancelled by user");
            }
            break;
        case MFMailComposeResultSaved:
            //NSLog(@"Mail saved");
            break;
        case MFMailComposeResultSent:
            //NSLog(@"Mail sent");
            if (listener != Nil) {
                listener->OnPostComplete();
            }
            break;
        case MFMailComposeResultFailed:
            //NSLog(@"Mail sent failure: %@", [error localizedDescription]);
            if (listener != Nil) {
                listener->OnPostFailure(3, String([error localizedDescription]));
            }
            break;
        default:
            break;
    }
    
	BBMonkeyAppDelegate *appDelegate=(BBMonkeyAppDelegate*)[[UIApplication sharedApplication] delegate];
    // Close the Mail Interface
    [appDelegate->viewController dismissViewControllerAnimated:YES completion:NULL];
}

@end

class NativeEmailSharer : public Object {

    SocialPostListener *_listener;

public:
    void NativeShareText(String subject, String text, String title);
    void Init();
    void SetListener(SocialPostListener *listener);
};

void NativeEmailSharer::NativeShareText(String subject, String text, String title) {
    MFMailComposeViewController *mc = [[MFMailComposeViewController alloc] init];
    mc.mailComposeDelegate = [SocialiteEmailDelegate delegateWithListener:_listener];
    [mc setSubject:subject.ToNSString()];
    [mc setMessageBody:text.ToNSString() isHTML:NO];

	BBMonkeyAppDelegate *appDelegate=(BBMonkeyAppDelegate*)[[UIApplication sharedApplication] delegate];
    
    // Present mail view controller on screen
    [appDelegate->viewController presentViewController:mc animated:YES completion:NULL];
    
}

void NativeEmailSharer::Init() {
}

void NativeEmailSharer::SetListener(SocialPostListener *listener) {
    _listener = listener;
}

