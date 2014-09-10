<?php

function ting_infomedia_no_library_form ($form, &$form_state) {


  if (isset($form_state['clicked_button']['#branch'])){
    $branch = $form_state['clicked_button']['#branch'];
    $form += ting_openformat_user_form_fields($form, $form_state, $branch);

  } else {
    $form = array(
      '#type' => 'container',
      '#attributes' => array(
        'class' => array('infomedia-search-group'),
      ),
    );
    $form['description_tag'] = array(
      '#theme' => 'html_tag',
      '#tag' => 'p',
      '#value' => t('no_favourite_library_description', array(), array('context' => 'ting_infomedia')),
    );
    $form['elements'] = bibdk_vejviser_form(array(), $form_state);

  if(isset($form_state['input']['openagency_query'])) {
    $query = check_plain($form_state['input']['openagency_query']);
    $result = bibdk_vejviser_execute_agency_search($query);
    $result = is_array($result) ? $result : array($result);
    $form += ting_infomedia_parse_agency_search_result($result);
  }
  }

  return $form;
}

function ting_infomedia_no_library_form_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}


function ting_infomedia_parse_agency_search_result($result) {
  $form = array();

  foreach ($result as $branch) {
    $form += ting_infomedia_view_agency_branch($branch);
  }
  return $form;
}


function ting_infomedia_execute_open_agency_search($query) {
  $result = bibdk_vejviser_execute_agency_search($query);
  return _ting_infomedia_get_markup_for_agency_search_result($result);
}

/**
 * Render the view of an agency branch and wrap it in an form element
 *
 * @param TingClientAgencyBranch $branch
 * @return mixed
 */
function ting_infomedia_view_agency_branch(TingClientAgencyBranch $branch) {
  global $language;

  $form['#title'] = $branch->getBranchName($language->language);

  $moreinfo = theme('ting_agency_more_info', array(
    'branch' => $branch,
    'openingHours' => theme('ting_agency_opening_hours', array('branch' => $branch)),
    'tools' => theme('ting_agency_tools', array('branch' => $branch)),
    'address' => theme('ting_agency_address', array('branch' => $branch)),
    'pickupallowed' => theme('ting_agency_pickup_allowed', array('branch' => $branch)),
    'librarydkcontact' => theme('ting_agency_librarydk_contact', array('branch' => $branch)),
    'contact' => theme('ting_agency_contact', array('branch' => $branch)),
  ));

  $classes = array();
  $classes[] = 'favourite_' . $branch->branchId;
  $classes[] = (isset($branch->orderLibrary) && $branch->orderLibrary == 'TRUE') ? 'order-agency' : NULL;

  $form[$branch->branchId]['action'] = array(
    '#value' => t('select_favourite_library', array(), array('context' => 'ting_infomedia')),
    '#name' => 'branch-' . $branch->branchId,
    '#type' => 'submit',
    '#submit' => array('ting_openformat_select_library_submit'),
    '#branch' => $branch,
  );

  $form[$branch->branchId]['library']['#markup'] = theme('ting_agency_library', array(
    'branchName' => $branch->getBranchName($language->language),
    'actions' => drupal_render($form[$branch->branchId]['action']),
    'add_classes' => $classes,
    'moreinfo' => $moreinfo,
    'branchid' => $branch->branchId
  ));

  // place branch on top if set as order-agency
  if (isset($branch->orderLibrary) && $branch->orderLibrary == 'TRUE') {
    $form[$branch->branchId]['#weight'] = -1;
  }
  return $form;
}


function ting_openformat_select_library_submit($form, &$form_state) {
  $form_state['rebuild'] = true;
}


/**
 * Render user fields;
 */
function ting_openformat_user_form_fields($form, $form_state, $branch) {
  $userData = NULL;
  $fields = bibdk_reservation_get_agency_fields();

  global $user;
  if (ding_user_is_provider_user($user)) {
    $fields = bibdk_favourite_user_form_fields($form, $form_state, $branch->branchId);
    $form['userParameters']['userfields'] = $fields;
  }
  else {
    $fields = ting_agency_userdata_form($form, $form_state, $fields, NULL, $branch->branchId);
    $form['userParameters']['userfields'] = $fields;
  }
  $form['userParameters']['userfields']['wrapper']['submit']['#submit'][] = 'ting_openformat_user_form_fields_submit';
  return $form;
}

/** Reload page to see articel
 * @param $form
 * @param $form_state
 */
function ting_openformat_user_form_fields_submit($form, &$form_state) {
  drupal_goto(drupal_get_path_alias($_GET['q']));
}

