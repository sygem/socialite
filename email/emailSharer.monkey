Import socialite

#If TARGET<>"android" And TARGET<>"ios"
'#Error "The email module is only available on the android and ios targets"
#End

#If TARGET="android"

Import "native/emailSharer.android.java"

#ElseIf TARGET="ios"

Import "native/emailSharer.ios.cpp"
#LIBS+="MessageUI.framework"

#End

Class EmailSharer Extends NativeEmailSharer Implements ISharer

    Method New()
        Init()
    End

    Method ShareText:Void(text:String)
        NativeShareText(subject, text, title)
    End

    Method SetPostListener:Void(listener:PostListener)
        SetListener(listener)
    End
    
    Method SetSubject:Void(subject:String)
        Self.subject = subject
    End
    
    Method SetTitle:Void(title:String)
        Self.title = title
    End
    
    Private
    Field subject:String = ""
    Field title:String = ""
    
End

Private

#If TARGET<>"android" And TARGET<>"ios"

Class NativeEmailSharer

    Private
    Method NativeShareText:Void(subject:String, text:String, title:String)
        Print("Share Text: " + text)
    End
    Method Init:Void()
    End
    Method SetListener:Void(listener:PostListener)
    End
End

#Else

Extern

Class NativeEmailSharer
    Private
    Method NativeShareText:Void(subject:String, text:String, title:String)
    Method Init:Void()
    Method SetListener:Void(listener:PostListener)

End

#End