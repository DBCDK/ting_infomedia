<?php

/**
 * @file
 * Field definition for ting_infomedia_manifestation_link is moved to
 * ting_infomedia_field module in order to be able to disable modules that
 * implements the field.
 * see http://drupal.org/node/1285540
 */

/**
 * Implements hook_field_formatter_info().
 */
function ting_infomedia_field_formatter_info() {
  return array(
    'ting_infomedia_link_default' => array(
      'label' => t('Default'),
      'field types' => array(
        'ting_infomedia_link',
      ),
    )
  );
}


/**
 * @TODO it this function used anywhere .. if not DELETE (the whole field definition
 *
 * Implements hook_field_formatter_view().
 */
function ting_infomedia_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  foreach ($items as $delta => $item) {
    if ($url = $entity->getInfomediaLink()) {
      $link = _ting_infomedia_get_link($entity->ding_entity_id, $url);
      $element[$delta] = array(
        '#theme' => 'ting_infomedia_button',
        '#ting_infomedia_button' => $link,
        '#attr' => array(
          'class' => array('infomedia-button', 'text-small', 'bibdk-popup-link'),
          'data-rel' => 'reservation',
        )
      );

    }
  }
  return $element;
}


/**
 * Set parameters for Infomedia link.
 * Check if user is a ding_user, and has access to Infomedia
 *
 * @param $id
 * @param $relation_uri
 * @return array
 */
function _ting_infomedia_get_link($id, $relation_uri) {
  $title = t('infomedia_link_text', array(), array('context' => 'ting_infomedia'));

  $link = array(
    '#href'   => _ting_infomedia_get_href($relation_uri),
    '#title'  => $title,
    '#weight' => 0,
  );


  return $link;
}


/**
 * @return string user status
 */
function _ting_infomedia_get_user_status() {
  global $user;

  if ( !ding_user_is_provider_user($user) ) {
    return 'user_is_not_provider_user';
  }

  $favourites = _ting_infomedia_get_favourites();

  if (empty($favourites)) {
    return 'user_has_no_favourite_library';
  }

  $library_code = $user_id = $user_pincode = NULL;

  foreach ($favourites as $fav) {
    $favourite = unserialize($fav);
    if ( $favourite->getOrderAgency() ) {
      $agency_main_id = $favourite->getAgencyMainId();
      $library_code = $favourite->getAgencyId();
      $user_id = $favourite->getUserId();
      $user_pincode = $favourite->getPinCode();
      break;
    }
  }

  if ( !$library_code && !$user_id && !$user_pincode ) {
    return 'user_no_userdata';
  }

  if ( !($library_code && $user_id && $user_pincode) ) {
    return 'user_incomplete_userdata';
  }

  $library_list = bibdk_forsrights_get_library_list('infomedia');

  if ( !in_array($library_code, $library_list) && !in_array($agency_main_id, $library_list)) {
    return 'not_infomedia_library';
  }

  $check_me = $favourite->getAgencyFields()->isBorrowerCheckRequired();
  if ($check_me) {
    $library_code = ( (int)$library_code === 0 ) ? $library_code : 'DK-' . $library_code;
    $borchk = bibdk_borchk_request($user_id, $user_pincode, $library_code, 'json', 'infomedia');
    $borchk_message = _ting_infomedia_borchk_parse_response($borchk->borrowerCheckResponse->requestStatus);
    if ( !empty($borchk_message) ) {
      return $borchk_message;
    }
  }



  return 'user_is_infomedia_user';
}


/**
 * Parse response from borchk and return messages
 *
 * @param string $response
 * @return string
 */
function _ting_infomedia_borchk_parse_response($response) {
  if ( is_object($response) ) {
    $response = $response->{'$'};
  }
  switch ($response) {
    case 'ok' :
      return '';
      break;
    case 'borrower_not_found' :
      return 'borchk_borrower_not_found';
      break;
    case 'borrower_not_in_municipality' :
      return 'borrower_not_in_municipality';
      break;
    case 'municipality_check_not_supported_by_library' :
      return 'municipality_check_not_supported_by_library';
      break;
    case 'error_in_request' :
    case 'service unavailable' :
    case 'borrowercheck_not_allowed' :
    default :
      return 'borchk_internal_error';
  }
}

/**
 * Get favourites
 * @return array
 */
function _ting_infomedia_get_favourites() {
  return bibdk_favourite_get_favourites();
}

/**
 * Determine user message, depending on logged in status
 *
 * @return String
 */
function _ting_infomedia_get_user_message($user_status, $relation_uri) {
  global $user;

  switch ( $user_status ) {
    CASE 'user_is_not_provider_user': // link to login
          return
            l(t($user_status, array(), array('context' => 'ting_infomedia.error')), 'user/login') .
            '<br/>' .
            l( t('infomedia_library_list', array(), array('context' => 'ting_infomedia')), 'overlay/infomedia/library_list', array('attributes' => array('data-rel' => 'bibdkFavorite', 'class' => array('bibdk-popup-link'))));
          break;
    CASE 'borchk_borrower_not_found':
    CASE 'user_no_userdata':
    CASE 'user_incomplete_userdata':
          return l( t($user_status, array(), array('context' => 'ting_infomedia.error')), 'user/' . $user->uid . '/bibdk_favourite_list');
          break;
    CASE 'user_has_no_favourite_library': // link to favorites
    CASE 'not_infomedia_library':
          return
            l( t($user_status, array(), array('context' => 'ting_infomedia.error')), 'user/' . $user->uid . '/bibdk_favourite_list') .
            '<br/>' .
            l( t('infomedia_library_list', array(), array('context' => 'ting_infomedia')), 'overlay/infomedia/library_list', array('attributes' => array('data-rel' => 'bibdkFavorite', 'class' => array('bibdk-popup-link'))));
          break;
    DEFAULT:
          return  t($user_status, array(), array('context' => 'ting_infomedia'));
  }

}

/**
 * Get infomedia link href: Local anchor for error message, or link to popup.
 *
 * @param $id
 * @param $user_status
 * @param $relation_uri
 *
 * @return string
 */
function _ting_infomedia_get_href($relation_uri) {
  $query_string = drupal_parse_url($relation_uri);
  return url('overlay/infomedia/' . $query_string['query']['faust']);
}


// translate status strings, so they are available when the module is enabled.
t('user_is_not_provider_user', array(), array('context' => 'ting_infomedia.error'));
t('user_has_no_favourite_library', array(), array('context' => 'ting_infomedia.error'));
t('user_no_userdata', array(), array('context' => 'ting_infomedia.error'));
t('user_incomplete_userdata', array(), array('context' => 'ting_infomedia.error'));
t('user_is_infomedia_user', array(), array('context' => 'ting_infomedia.error'));
t('infomedia_library_list', array(), array('context' => 'ting_infomedia.error'));
t('municipality_check_not_supported_by_library', array(), array('context' => 'ting_infomedia.error'));
t('borrower_not_in_municipality', array(), array('context' => 'ting_infomedia.error'));

t('borchk_borrower_not_found', array(), array('context' => 'ting_infomedia.error'));
t('borchk_internal_error', array(), array('context' => 'ting_infomedia.error'));
