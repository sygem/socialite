socialite - Twitter
===================

Create Your Twitter App
-----------------------

- https://apps.twitter.com/
- Add new app
- Set access level to "read and write"
- Note the API keys

Final Steps
-----------

In your main Monkey file, add the following definitions:

    #TWITTER = true
    #TWITTER_CONSUMER_KEY="<YOUR CONSUMER KEY>"
    #TWITTER_CONSUMER_SECRET="<YOUR CONSUMER SECRET>"
    
Then create the TwitterSharer:

    Local t := New TwitterSharer()
    t.ShareText("This is a tweet")


Callbacks
---------

You can listen for the result of the sharing:

    t.SetPostListener(new TwitterListener())

And the listener looks a bit like this:

    Class TwitterListener Extends SocialPostListener
       Method OnPostComplete:Void()
       End
       Method OnPostFailure:Void(error:Int, message:String)
       End
    End