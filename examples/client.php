<?php

use Spiral\Goridge\RPC;

require __DIR__ . '/../vendor/autoload.php';

$rpc = RPC::connect('tcp://localhost:6001');

$req = base64_encode(\random_bytes(65535));

echo ($req == $rpc->call("App.Hi", $req) ? 'COOL' : 'NO COOL') . \PHP_EOL;
