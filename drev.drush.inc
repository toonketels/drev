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
