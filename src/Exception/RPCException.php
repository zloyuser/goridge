<?php

declare(strict_types=1);

namespace Spiral\Goridge\Exception;

class RPCException extends GoridgeException
{
    /**
     * @return self
     */
    public static function noControl(): self
    {
        return new self('RPC control frame is missing.');
    }

    /**
     * @return self
     */
    public static function noResponse(): self
    {
        return new self('RPC response frame is missing.');
    }

    /**
     * @return self
     */
    public static function controlMismatch(): self
    {
        return new self('RPC method call mismatch.');
    }
}
