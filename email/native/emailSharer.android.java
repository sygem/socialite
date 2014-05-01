class NativeEmailSharer {

    private SocialPostListener listener;

    public void Init() {
    }
    
    public void SetListener(SocialPostListener listener) {
        this.listener = listener;
    }
    
    public void NativeShareText(String subject, String text, String title) {
        Context context = BBAndroidGame.AndroidGame().GetActivity();
        final Intent intent = new Intent(Intent.ACTION_SEND);
        
        intent.setType("text/plain");
        intent.putExtra(Intent.EXTRA_SUBJECT, subject);
        intent.putExtra(Intent.EXTRA_TEXT, text);
        if (title != null && title.length() > 0) {
            context.startActivity(Intent.createChooser(intent, title));
        } else {
            context.startActivity(intent);
        }
    }

}
