<?php

declare(strict_types=1);

namespace Spiral\Goridge\Tests;

use PHPUnit\Framework\TestCase;
use Spiral\Goridge\Exception;
use Spiral\Goridge\Relay;
use Spiral\Goridge\RPC;

abstract class RPCTest extends TestCase
{
    public function testPingPong()
    {
        self::assertSame('pong', $this->call('Service.Ping', 'ping'));
    }

    public function testPingNull()
    {
        self::assertSame('', $this->call('Service.Ping', 'not-ping'));
    }

    public function testNegate()
    {
        self::assertSame(-10, $this->call('Service.Negate', 10));
    }

    public function testNegateNegative()
    {
        self::assertSame(10, $this->call('Service.Negate', -10));
    }

    public function testLongEcho()
    {
        $payload = $this->payload(5);

        $resp = $this->call('Service.Echo', $payload);

        self::assertSame(\strlen($payload), \strlen($resp));
        self::assertSame(\md5($payload), \md5($resp));
    }

    public function testConvertException()
    {
        self::expectException(Exception\ServiceException::class);
        self::expectExceptionMessage('{rawData} request for <*string Value>');

        $payload = $this->payload(5);

        $resp = $this->call('Service.Echo', $payload, true);

        self::assertSame(\strlen($payload), \strlen($resp));
        self::assertSame(\md5($payload), \md5($resp));
    }

    public function testRawBody()
    {
        $payload = $this->payload(100);

        $resp = $this->call('Service.EchoBinary', $payload, true);

        self::assertSame(\strlen($payload), \strlen($resp));
        self::assertSame(\md5($payload), \md5($resp));
    }

    public function testLongRawBody()
    {
        $payload = $this->payload(1000);

        $resp = $this->call('Service.EchoBinary', $payload, true);

        self::assertSame(\strlen($payload), \strlen($resp));
        self::assertSame(\md5($payload), \md5($resp));
    }

    public function testPayload()
    {
        $resp = $this->call('Service.Process', [
            'name'  => 'spiral',
            'value' => 18,
        ]);

        self::assertSame([
            'name'  => 'SPIRAL',
            'value' => -18,
        ], $resp);
    }

    public function testBadPayload()
    {
        self::expectException(Exception\ServiceException::class);
        self::expectExceptionMessage('{rawData} request for <*main.Payload Value>');

        $this->call('Service.Process', 'raw', true);
    }

    public function testPayloadWithMap()
    {
        $resp = $this->call('Service.Process', [
            'name'  => 'spiral',
            'value' => 18,
            'keys'  => [
                'key'   => 'value',
                'email' => 'domain',
            ],
        ]);

        self::assertIsArray($resp['keys']);
        self::assertArrayHasKey('value', $resp['keys']);
        self::assertArrayHasKey('domain', $resp['keys']);

        self::assertSame('key', $resp['keys']['value']);
        self::assertSame('email', $resp['keys']['domain']);
    }

    public function testBrokenPayloadMap()
    {
        self::expectException(Exception\ServiceException::class);
        self::expectExceptionMessageRegExp('#.*cannot unmarshal number.*#');

        $this->call('Service.Process', [
            'name'  => 'spiral',
            'value' => 18,
            'keys'  => 1111,
        ]);
    }

    public function testJsonException()
    {
        self::expectException(Exception\JSONException::class);
        self::expectExceptionMessageRegExp('#.*JSON error.*#');

        /** @noinspection PhpUnhandledExceptionInspection */
        $this->call('Service.Process', \random_bytes(256));
    }

    /**
     * @noinspection PhpDocMissingThrowsInspection
     *
     * @param int $multiplier
     *
     * @return string
     */
    private function payload(int $multiplier): string
    {
        /** @noinspection PhpUnhandledExceptionInspection */
        return \base64_encode(\random_bytes(Relay::BUFFER_SIZE * $multiplier));
    }

    /**
     * @param string $method
     * @param mixed  $payload
     * @param bool   $raw
     * 
     * @return mixed
     */
    private function call(string $method, $payload, bool $raw = false)
    {
        return $this->makeRPC()->call($method, $payload, $raw);
    }

    /**
     * @return RPC
     */
    abstract protected function makeRPC(): RPC;
}
