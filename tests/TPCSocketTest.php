<?php

declare(strict_types=1);

namespace Spiral\Goridge\Tests;

use Spiral\Goridge\RPC;

class TPCSocketTest extends RPCTest
{
    /**
     * {@inheritDoc}
     */
    protected function makeRPC(): RPC
    {
        return RPC::connect('tcp://127.0.0.1:7079');
    }
}
