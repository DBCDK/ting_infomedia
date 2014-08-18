<a href="<?php print $ting_infomedia_button['#href'] ?>"
   class="<?php print $ting_infomedia_button['#popup'] ?><?php print $ting_infomedia_button['#popover'] ?>infomedia-button"
   data-rel="infomedia">
  <i class="icon--darkgrey icon--addbasket"></i>
  <span class="link-infomedia infomedia-title" data-pid="<?php print $ting_infomedia_button['#data-pid'] ?>"><?php print $ting_infomedia_button['#title'] ?></span>
</a>

<?php if (!empty($ting_infomedia_button['#popover'])): ?>
  <div class="popover element-wrapper visuallyhidden infomedia-wrapper">
    <p class="user-msg"><?php print $ting_infomedia_button['#user-message'] ?></p>
    <p class="close icon icon-left icon-red-x">&nbsp;</p>
  </div>
<?php endif; ?>
