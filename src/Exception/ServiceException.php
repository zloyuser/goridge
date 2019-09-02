<?php

declare(strict_types=1);

namespace Spiral\Goridge\Exception;

use Spiral\Goridge\Frame;

class ServiceException extends RPCException
{
    /**
     * @param Frame $frame
     *
     * @return self
     */
    public static function forResponse(Frame $frame): self
    {
        return new self($frame->body);
    }
}
