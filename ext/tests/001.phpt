--TEST--
Check if goridge is loaded
--SKIPIF--
<?php
if (!extension_loaded('goridge')) {
	echo 'skip';
}
?>
--FILE--
<?php
echo 'The extension "goridge" is available';
?>
--EXPECT--
The extension "goridge" is available
