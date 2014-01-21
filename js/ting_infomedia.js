(function($) {

    Drupal.behaviors.popUpPrint = {
        attach: function(context, settings) {
            $('.pop-up-print').click(function() {
               window.print();
               return false;
            });
        }
    };


    Drupal.behaviors.recommendation = {
        attach:function (context) {
            $('.article-tabs li', context).click(function(i, element) {
                var val = $(this).attr('data-article-id');
                $('.item-list .infomedia-article', context).each(function (i, element) {
                    $(this).addClass("visuallyhidden");
                    if ( $(this).attr('data-article-id') == val ) {
                        $(this).removeClass("visuallyhidden");
                    }
                });
            });
        },
        detach:function (context) {
            //
        }
    };

} (jQuery));
