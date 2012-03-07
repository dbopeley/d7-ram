<?php

/**
 * @file
 * Functions that are only called on the admin pages.
 */

/**
 * Overriding system settings form.
 */
function ram_system_settings_form($form, $automatic_defaults = TRUE) {
  $form['actions']['#type'] = 'container';
  $form['actions']['#attributes']['class'][] = 'form-actions';
  $form['actions']['#weight'] = 100;
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));

  if ($automatic_defaults) {
    $form = _system_settings_form_automatic_defaults($form);
  }

  if (!empty($_POST) && form_get_errors()) {
    drupal_set_message(t('The settings have not been saved because of the errors.'), 'error');
  }
  $form['#submit'][] = 'system_settings_form_submit';
  // By default, render the form using theme_system_settings_form().
  if (!isset($form['#theme'])) {
    $form['#theme'] = 'system_settings_form';
  }
  return $form;
}

/**
 * Module settings form.
 */
function ram_admin_settings() {

  
	$form['ram_content_type_fs'] = array(
	  '#type' => 'fieldset', 
	  '#title' => t('Property Content Type'), 
	  '#collapsible' => TRUE, 
	  '#collapsed' => FALSE,
	);
  
	

    
    
  foreach (node_type_get_types() as $nt) {

    if (isset($nt->name)) {
    	$nt_type = $nt->type;
      $nt_options[$nt_type] = $nt->name;
      }
  	}

    
    
    $form['ram_content_type_fs']['ram_content_type'] = array(
      '#type'           => 'select',
      '#title'          => t( 'Chose your Content Type.' ),
      '#description'    => t( 'Chose the content type you wish to map an association to. After you select this cotent type, please press save configuration and the system will lock your content type, and start the mapping process.' ),
      '#options'        => $nt_options,
      '#default_value'  => variable_get('ram_content_type', null),
      '#required'       => true,
      '#weight'         => -49
    );
  
	$form['ram_feed_fs'] = array(
	  '#type' => 'fieldset', 
	  '#title' => t('Configure RAM Feed'), 
	  '#collapsible' => TRUE, 
	  '#collapsed' => FALSE,
	);
  
	$form['ram_feed_fs']['ram_feed'] = array(
		'#type' => 'textfield', 
		'#title' => t('Feed URL'), 
		'#default_value' => variable_get('ram_feed', null), 
		'#size' => 60, 
		'#maxlength' => 128, 
		'#required' => TRUE
	);
	
	$form['ram_feed_fs']['ram_feed_keys_count'] = array(
		'#type' => 'select', 
		'#title' => t('Number of Field Keys'), 
	  '#options'=> range(1, 100),
    '#default_value'  => variable_get('ram_feed_keys_count', null),
		'#required' => TRUE
	);
	
	
	   $keyscount = variable_get('ram_feed_keys_count', null);




    if ($keyscount != null ) {
    	
			$i = 1;
	    while($i <= $keyscount){
	    
	    		$form['ram_feed_fs']['ram_feed'] = array(
		'#type' => 'textfield', 
		'#title' => t('Feed URL'), 
		'#default_value' => variable_get('ram_feed', null), 
		'#size' => 60, 
		'#maxlength' => 128, 
		'#required' => TRUE
	);
	    	
	    	
	    	
	    	
	    	$i++;
	    }
    
    }
  
  
 //$form = array_merge($form, _ram_setupContentTypeForm());

  return ram_system_settings_form($form, FALSE);
}

/**
 * Implements hook_validate().
 */
function ram_admin_settings_validate($form, &$form_state) {
  $error = array();

  if (!empty($error)) {
    $error_message = '';
    if (count($error) > 1) {
      foreach ($error as $e) {
        $error_message .= '<li>' . $e . '</li>';
      }
      $error_message = t('Files not found') . ': <ul>' . $error_message . '</ul>';
    }
    else {
      $error_message = t('File not found') . ': ' . $error[0];
    }
    form_set_error('ram_slp', $error_message);
  }
}


  /**
   * @name _setupContentTypeForm()
   * return array the form API for this form, so other modules can share it
   * @todo = make this more API like
   */
  function _ram_setupContentTypeForm() {

    $CCKtype = variable_get('ram_xml_content_type', null);
   $cckMap = variable_get('ram_xml_cck_map', array());




    if ($CCKtype != null ) { //get the options available to map.

      $tokens_raw = token_get_list( 'property' );
      unset( $tokens_raw['global'] );

      $tokens = array( 'NULL' => 'No Mapping Assigned' );
      foreach ( $tokens_raw['Property Suite'] as $key=> $value ) {

        $tokens[trim($key)] = trim($key);

      }

      //@todo - this variable cannot have RAM in it!!
      $tokens[variable_get( 'ram_property_image_key', 'imagesavailable' )] = 'Image_Field'; //Add imagefield support @todo add in File Field support for attachments - This needs to be left at the end, because it is how I pull it from the array, using end()
      asort( $tokens );
      $type = content_types($CCKtype);

      $name = 'property_content_type_' . $CCKtype .'_node_title';

      $variable = array( $name );

      $form['property_content_type']['setup'][0][ $name ] = array( //set up the node title, since this is always there
        '#type'           => 'textfield',
        '#title'          => t( 'Node Title' ),
        '#default_value'  => variable_get( $name, 'NULL' ),
        '#description'    => t( 'You can add any tokens together to create a title for this node, all spaces and non-ascii characters will automatically replace with the replacement character you select below. ex: [Address]_[City]_[State]_[Zip]' ),
        '#weight'         => -48,
        '#group'          => 'property_content_type'
      );

      $options = array(
        '#HYPHEN' => t( 'Hyphen - ' ),
        '#UNDERSCORE' => t( 'Underscore _ ' ),
      );
      $form['property_content_type']['setup'][0]['property_content_type_token_ascii_replacement'] = array(
        '#type'           => 'select',
        '#title'          => t( 'Replace ASCII Characters.' ),
        '#description'    => t( 'You may chose from a hyphen (-) or underscore (_) as these are the best for SEO purposes.' ),
        '#options'        => $options,
        '#default_value'  => variable_get( 'property_content_type_token_ascii_replacement', '#HYPHEN' ),
        '#required'       => true,
        '#weight'         => -47,
        '#group'          => 'property_content_type'
      );

      //Location fields
      if ( module_exists( 'location' ) ) {

        $location_tokens = variable_get( 'location_settings_node_' . $CCKtype, array() ); //location fields enabled
        unset( $location_tokens['form']['fields']['name'] ); //uset the name fields, since it is hard to support

        if ( !empty( $location_tokens ) ) {

          foreach ( $location_tokens['form']['fields'] as $key => $value ) {

            if ( $value['collect'] == 1 ) { //if it is set to collect this value, show it

              $name = 'property_content_type_' . $CCKtype .'_location_' .$key;
              $variable[] = $name;

              $form['property_content_type']['setup'][1][ $name ] = array(
                '#type'           => 'select',
                '#title'          => t( 'Location !name', array( '!name' => ucwords( $key ) ) ),
                '#options'        => $tokens,
                '#default_value'  => variable_get( $name, 'NULL' ),
                '#weight'         => $value['weight'],
                '#group'          => 'property_content_type'
              );
            }
          }
        }
      }

      $fields = $type['fields'];
      $fieldMap = variable_get('ram_xml_field_map', array());

      sort($fieldMap);

      array_unshift($fieldMap, 'None');
      $tokens = array_combine(array_values($fieldMap), array_values($fieldMap));


      #
      # Mapping for each CCK field to one XML field
      #
      foreach ( $fields as $field ) {

        $name = "ram_import_$CCKtype-" . $field['field_name'];
        $variable[] = $name;

        #
        # Found ['widget']['label'] is better label than ['field_name']
        #
        //$form['property_content_type']['keymap']["#tree"] = TRUE;
        $form['property_content_type']['setup'][2][$name] = array(
          '#type'           => 'select',
          '#title'          => $field['widget']['label'],
          '#options'        => $tokens,
          '#default_value'  => $cckMap[$field['field_name']],
          '#weight'         => $field['widget']['weight'],
          '#group'          => 'property_content_type'
        );

      }

      variable_set( 'property_content_type_field_names', $variable ); //regen field names on the fly

    }
    
   

   // $form['#pre_render'][] = 'vertical_tabs_form_pre_render'; //enabled vertical tabs if the module is installed

    return $form;

  }