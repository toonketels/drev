<?php

/**
 * Recurse over entire menu tree and updates path each menu item.
 */
function drev_sync_path_menu_update_menu($tree, $parent_alias = NULL) {
  
  foreach($tree as $item) {
    
    // Update the item
    $new_parent_alias = drev_sync_path_menu_update_menu_item($item, $parent_alias);
    
    // And go deeper if we can...
    if(!empty($item['below'])) {
      drev_sync_path_menu_update_menu($item['below'], $new_parent_alias);
    }
  }
  
}


/**
 * Updates the path of a menu item to mimic the menu structure.
 */
function drev_sync_path_menu_update_menu_item($item, $parent_alias = NULL) {
  
  require_once(drupal_get_path('module', 'pathauto') . '/pathauto.inc');
  
  $link = $item['link'];
  $alias = '';
  
  // Start alias with parent's alias if we have a parent...
  if($parent_alias) {
    $alias .= $parent_alias . '/';
  }
  
  // For nodes use node the node title to generate alias..
  if($link['router_path'] == 'node/%') {
    $nid = end(explode('/', $link['link_path']));
    $node = node_load($nid);
    $title = $node->title;
    $alias .= pathauto_cleanstring($title);
    
  // For others, use the menu title.
  } else {
    $alias .= pathauto_cleanstring($link['link_title']);
  }
  
  // Delete the existing alias if we found one.
  $existing_alias = _pathauto_existing_alias_data($link['link_path']);
  if($existing_alias) {
    foreach($existing_alias as $pid => $old_alias) {
      path_delete($pid);
    }
  }

  // Save the new path...
  $path = array(
    'source' => $link['link_path'],
    'alias' => $alias,
  );
  path_save($path);
  
  drush_log(dt("Updated path for menu item {$link['link_path']} to {$alias}."), 'success');
  
  // Return so new alias can be set as the parent alias for this
  // menu item's children.
  return $alias;
}