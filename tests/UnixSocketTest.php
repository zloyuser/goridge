<?php

declare(strict_types=1);

namespace Spiral\Goridge\Tests;

use Spiral\Goridge\RPC;

class UnixSocketTest extends RPCTest
{
    /**
     * {@inheritDoc}
     */
    protected function makeRPC(): RPC
    {
        return RPC::connect('unix://server.sock');
    }
}
