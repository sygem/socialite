class SocialPostListener : public Object {

public: 

    virtual void OnPostComplete() {}
    virtual void OnPostFailure(int errorCode, String message) {}

};

class SocialLogInOutListener : public Object {

public: 

    virtual void OnLoginComplete() {}
    virtual void OnLogoutComplete() {}
    virtual void OnLoginFailure(int errorCode, String message) {}

};

class SocialInitializationListener : public Object {

public: 

    virtual void OnInitializeComplete() {}
    virtual void OnInitializeFailure(int errorCode, String message) {}

};

