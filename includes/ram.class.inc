<?php
/**
 * @name blank
 * @param string $path = the module's internal path, as defined in our .module file
 * This is the default module class, this just helps us keep our items separated from Drupal
 * and keeps the module hooks in the .module file so that we know our work is as separated from Drupal's hooks
 * @todo - add any more path variables we need here.
 */
class ram {
  
  //Build any object references we need here - when we include this file, PHP will looks for the construct and give us this object automatically.
  function __construct() {
    //Constructor functions.
  }
  
  //When we are done with our object, or do not reference it anymore PHP will automatically try and kill it off from memory if we have this set.
  function __destruct() {
    //Unset from memory.
    unset($this);
  }

 
  
}