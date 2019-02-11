<?php

/**
 * @file
 * Drush invocation of a consolidate OCR batch.
 */

/**
 * Implements hook_drush_command().
 */
function islandora_consolidated_ocr_drush_command() {
  $commands = array();

  $commands['islandora_consolidated_ocr_generate'] = array(
    'aliases' => array('icog'),
    'description' => dt('Consolidates existing OCR datastreams onto a parent without triggering re-generation.'),
    'drupal depdendencies' => array(
      'islandora_paged_content',
    ),
    'options' => array(
      'parent' => array(
        'description' => dt('The parent paged content object to have OCR consolidated on.'),
        'required' => TRUE,
      ),
    ),
  );
  return $commands;
}

/**
 * Validation to ensure the parent exists and has pages.
 */
function drush_islandora_consolidated_ocr_generate_validate() {
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');

  // XXX: Due to how Drush bootstrapping works, the connection may be created
  // without credentials (when your site's front page is
  // 'islandora/object/some:object', for example). Resetting to ensure a new
  // connection gets created should fix it.
  drupal_static_reset('islandora_get_tuque_connection');

  $parent = drush_get_option('parent');
  $object = islandora_object_load($parent);
  if (!$object) {
    return drush_set_error('islandora_consolidated_ocr', dt('The specified parent object (@object) does not exist.', array(
      '@object' => $parent,
    )));
  }
  if (empty(islandora_paged_content_get_pages($object))) {
    return drush_set_error('islandora_consolidated_ocr', dt('The specified parent object (@object) has no children or is not a paged content object.', array(
      '@object' => $parent,
    )));
  }

}

/**
 * Drush command to kick off the consolidation batch.
 */
function drush_islandora_consolidated_ocr_generate() {
  module_load_include('inc', 'islandora_consolidated_ocr', 'includes/batch');
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');

  // XXX: Due to how Drush bootstrapping works, the connection may be created
  // without credentials (when your site's front page is
  // 'islandora/object/some:object', for example). Resetting to ensure a new
  // connection gets created should fix it.
  drupal_static_reset('islandora_get_tuque_connection');

  $parent = drush_get_option('parent');
  $object = islandora_object_load($parent);
  $pages = array_keys(islandora_paged_content_get_pages($object));
  $batch = islandora_consolidated_ocr_batch($object, $pages);
  batch_set($batch);
  drush_log(dt('Beginning consolidation of OCR for @object.', array(
    '@object' => $parent,
  )));
  drush_backend_batch_process();
}
