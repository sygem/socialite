socialite - Email
=================

Final Steps
-----------

In your main Monkey file, add the following definition:

    #EMAIL = true
    
Then create the EmailSharer:

    Local e := New EmailSharer()
    e.SetSubject("Subject") // optional
    e.SetTitle("Title") // optional
    e.ShareText("This is an email")


Callbacks
---------

You can listen for the result of the sharing:

    e.SetPostListener(new EmailListener())

And the listener looks a bit like this:

    Class EmailListener Extends SocialPostListener
       Method OnPostComplete:Void()
       End
       Method OnPostFailure:Void(error:Int, message:String)
       End
    End