<?php

/** Form if user has no order library
 * @param $form
 * @param $form_state
 * @return array|mixed
 */
function ting_infomedia_no_library_form($form, &$form_state) {
  if (isset($form_state['clicked_button']['#branch'])) {
    $branch = $form_state['clicked_button']['#branch'];
    $form += ting_infomedia_user_form_fields($form, $form_state, $branch);
  }
  else {
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

    if (isset($form_state['input']['openagency_query'])) {
      $query = check_plain($form_state['input']['openagency_query']);
      $result = bibdk_vejviser_execute_agency_search($query);
      $result = is_array($result) ? $result : array($result);
      $form += ting_infomedia_parse_agency_search_result($result);
    }
  }
  return $form;
}

/** Rebuild form_state
 * @param $form
 * @param $form_state
 */
function ting_infomedia_no_library_form_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}

/** view agency search result
 * @param $result
 * @return array|mixed
 */
function ting_infomedia_parse_agency_search_result($result) {
  $form = array();

  foreach ($result as $branch) {
    $action = ting_infomedia_get_agency_action($branch);
    $form += ting_agency_view_agency_branch($branch, $action);
  }
  return $form;
}

/** execute agency search
 * @param $query
 * @return mixed
 */
function ting_infomedia_execute_open_agency_search($query) {
  $result = bibdk_vejviser_execute_agency_search($query);
  return _ting_infomedia_get_markup_for_agency_search_result($result);
}

/** Action for agency search result view
 * @param $branch
 * @return array
 */
function ting_infomedia_get_agency_action($branch) {
  return array(
    '#value' => t('select_favourite_library', array(), array('context' => 'ting_infomedia')),
    '#name' => 'branch-' . $branch->branchId,
    '#type' => 'submit',
    '#submit' => array('ting_infomedia_select_library_submit'),
    '#branch' => $branch,
  );
}

/** rebuild form_state
 * @param $form
 * @param $form_state
 */
function ting_infomedia_select_library_submit($form, &$form_state) {
  $form_state['rebuild'] = true;
}


/**
 * Render user fields;
 * TODO: Refaktor to use same form here, bibdk_favourites and bibdk_reservation
 */
function ting_infomedia_user_form_fields($form, $form_state, $branch) {
  if (ding_user_is_provider_user()) {
    $fields = bibdk_favourite_user_form_fields($form, $form_state, $branch->branchId);
    $form['userParameters']['userfields'] = $fields;
  }
  else {
    $fields = bibdk_reservation_get_agency_fields();
    $fields = ting_agency_userdata_form($form, $form_state, $fields, NULL, $branch->branchId);
    $form['userParameters']['userfields'] = $fields;
  }
  $form['userParameters']['userfields']['wrapper']['submit']['#submit'][] = 'ting_infomedia_user_form_fields_submit';
  $form['#validate'] = $form['userParameters']['userfields']['#validate'];
  return $form;
}

/** Reload page to view article
 *
 * @param $form
 * @param $form_state
 */
function ting_infomedia_user_form_fields_submit($form, &$form_state) {
  drupal_goto(drupal_get_path_alias($_GET['q']));
}

