(function($) {

  Drupal.behaviors.popUpPrint = {
    attach: function(context, settings) {
      $('.pop-up-print').click(function() {
       window.print();
       return false;
      });
    }
  };

} (jQuery));
