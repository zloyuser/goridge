<?php

declare(strict_types=1);

namespace Spiral\Goridge\Exception;

/**
 * @todo Remove this after PHP 7.3 support
 */
final class JSONException extends GoridgeException
{
    /**
     * @return self
     */
    public static function last(): self
    {
        return new self(\sprintf("JSON error: %s.", \json_last_error_msg()));
    }
}
