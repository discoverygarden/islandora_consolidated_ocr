<?php

/**
 * @file
 * Handles the creation and running of the consolidation batch.
 */

/**
 * Consolidate OCR batch.
 *
 * @param AbstractObject $object
 *   An AbstractObject representing an object within Fedora.
 * @param array $pages
 *   All the pages to perform OCR consolidation on.
 *
 * @return array
 *   An array defining the batch to be ran.
 */
function islandora_consolidated_ocr_batch(AbstractObject $object, array $pages) {
  // Use the paged_content batch as the base.
  module_load_include('inc', 'islandora_paged_content', 'includes/batch');
  $batch = islandora_paged_content_create_ocr_batch($object, $pages, array('aggregate_ocr' => TRUE));

  $batch['operations'] = array();
  $batch['operations'][] = array('islandora_consolidated_ocr_grab_page_ocr_operation', array($pages));
  $batch['operations'][] = array('islandora_paged_content_create_ocr_paged_content_batch_operation', array($object->id));
  return $batch;
}
