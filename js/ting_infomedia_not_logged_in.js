(function($) {
  Drupal.behaviors.infomediaNotLoggedIn = {
    attach:function(context) {
       $('a.infomedia-button',context).click(function(e){
        e.preventDefault();
        $('#infomedia_toggle_link').toggleClass('hidden');
      })
    }
  }
} (jQuery));
