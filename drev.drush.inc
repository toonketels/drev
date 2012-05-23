<?php
/**
 * @file
 * 
 * Drush commands.
 */


/**
 * Implements hook_drush_command().
 */
function drev_drush_command() {
  $items = array();

  $items['drev-create-users'] = array(
    'description' => 'Creates a user per role with name/password rolename.',
    'bootstrap' => DRUSH_BOOTSTRAP_LEVEL_FULL,
  );

  $items['drev-delete-users'] = array(
    'description' => 'Deletes the demo users which have name/password corresponding to rolename.',
    'bootstrap' => DRUSH_BOOTSTRAP_LEVEL_FULL,
  );
  
  $items['drev-sync-path-menu'] = array(
    'description' => 'Rebuilds the path aliases the be in sync with menu directory structure.',
    'bootstrap' => DRUSH_BOOTSTRAP_LEVEL_FULL,
  );

  return $items;
}


/**
 * Drush callback.
 */
function drush_drev_create_users() {

  $roles = user_roles(TRUE);

  foreach($roles as $role_id => $role) {
    $name = str_replace(' ', '_', strtolower($role));

    if(!user_load_by_name($name)) {
      $user = (object) array(
        'name' => $name,
        'password' => $name,
        'roles' => array($role_id => $role),
        'mail' => $name . '@example.com',
      );

      user_save($user);

      echo "Created user $name\n";
    } else {
      echo "User $name already exists\n";
    }
  }
}


/**
 * Drush callback.
 */
function drush_drev_delete_users() {

  $roles = user_roles(TRUE);

  foreach($roles as $role_id => $role) {
    $name = str_replace(' ', '_', strtolower($role));

    if($user = user_load_by_name($name)) {
      user_delete($user->uid);

      echo "Deleted user $name\n";
    } else {
      echo "User $name does not exists\n";
    }
  }
}


/**
 * Drush callback.
 *
 * Updates the path of items in menu to mimic that of menu structure.
 *
 * Useful when pages in menu have alias pattern set to mimic trail but lot's
 * of changes in menu have made the aliases out of sync. Instead of needing to
 * update each node manually we use this function to "resync" aliases and menu
 * structure.
 *
 * Using pathauto's functionality to mass create aliases did not go so well.
 */
function drush_drev_sync_path_menu() {
  
  require_once(dirname(__FILE__) . '/includes/drev.sync_path_menu.php');
  
  if(!module_exists('pathauto')) {
    drush_set_error(dt('Pathauto module is required for this command to work.'));
    return;
  }
  
  $menus = menu_get_menus();
  $menu = drush_choice($menus, dt('Which menu would you like to rebuiild?'));

  $menu_tree = menu_build_tree($menu);
  drev_sync_path_menu_update_menu($menu_tree);
}