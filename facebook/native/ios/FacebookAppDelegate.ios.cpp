@implementation BBMonkeyAppDelegate (FacebookAdditions)

-(BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation {
    return [FBSession.activeSession handleOpenURL:url];
}
         
-(void)applicationDidBecomeActive:(UIApplication *)application {
    // do what the original method was doing
	game->ResumeGame();
    // Handle the user leaving the app while the Facebook login dialog is being shown
    // For example: when the user presses the iOS "home" button while the login dialog is active
    NSLog(@"FBAppCall handleDidBecomeActive");
    [FBAppCall handleDidBecomeActive];
}

@end
