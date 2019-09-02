<?php

use Spiral\Goridge\RPC;

require __DIR__ . '/../vendor/autoload.php';

$rpc = RPC::connect('tcp://localhost:6001');

echo $rpc->call("App.Hi", "World") . \PHP_EOL;
