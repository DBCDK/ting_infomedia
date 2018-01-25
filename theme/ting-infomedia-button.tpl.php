<a href="<?php print $ting_infomedia_button['#href'] ?>" <?php print drupal_attributes($attr); ?>>
  <span class="icon icon-left icon-darkgrey-infomedia">&nbsp;</span>
  <span class="link-infomedia infomedia-title"><?php print $ting_infomedia_button['#title'] ?></span>
  <?php if(isset($ting_infomedia_button['#hidden_link'])):?>
    <?php print drupal_render($ting_infomedia_button['#hidden_link']);?>
  <?php endif?>
</a>
