<?php

declare(strict_types=1);

namespace Spiral\Goridge\Exception;

final class StreamException extends GoridgeException
{
    /**
     * @param mixed $stream
     *
     * @return self
     */
    public static function unsupportedResource($stream): self
    {
        $type = \is_resource($stream) ? \get_resource_type($stream) : \gettype($stream);

        return new self(\sprintf('Expected `stream` resource. Got: %s.', $type));
    }

    /**
     * @param string $scheme
     *
     * @return self
     */
    public static function unsupportedTransport(string $scheme)
    {
        return new self("Unsupported stream transport: {$scheme}");
    }

    /**
     * @param string $mode
     *
     * @return self
     */
    public static function unsupportedMode(string $mode): self
    {
        return new self(\sprintf('Stream must be %s.', $mode === 'r' ? 'readable' : 'writable'));
    }

    /**
     * @param string $uri
     * @param string $error
     * @param int    $code
     *
     * @return self
     */
    public static function connectFailed(string $uri, string $error, int $code): self
    {
        return new self(\sprintf("Connection to %s failed: %s.", $uri, $error), $code);
    }

    /**
     * @return self
     */
    public static function readFailed(): self
    {
        return new self("Error reading data from stream.");
    }

    /**
     * @return self
     */
    public static function writeFailed(): self
    {
        return new self("Error writing data to stream.");
    }
}
