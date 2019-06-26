<?php
$br = (php_sapi_name() == "cli")? "":"<br>";

if(!extension_loaded('roaring_bitmap')) {
	dl('roaring_bitmap.' . PHP_SHLIB_SUFFIX);
}
$module = 'roaring_bitmap';
$functions = get_extension_funcs($module);
echo "Functions available in the test extension:$br\n";
if(!empty($functions)){
	foreach($functions as $func) {
	    echo $func."$br\n";
	}
}
echo "$br\n";

?>
