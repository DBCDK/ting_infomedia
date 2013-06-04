<div class="infomedia-article">

  <div id="header" class="clearfix">

    <div class="logo-image">
      <?php print theme('image', array('path' => drupal_get_path('theme', 'bibdk_theme') . '/img/dbc-logo-popup.png')); ?>
    </div>

    <div class="infomedia-image">
      <?php print theme('image', array('path' => drupal_get_path('module', 'ting_infomedia') . '/img/ting_infomedia_logo.jpg')); ?>
    </div>

    <div class="pop-up-print">
      <?php echo $variables['element']['#print']; ?>
    </div>

  </div>

  <?php echo $variables['element']['#markup']; ?>

  <div class="infomedia-footer">

    <p><?php print t('infomedia_copyright_notice', array(), array('context' => 'ting_infomedia')); ?></p>

  </div>

</div>