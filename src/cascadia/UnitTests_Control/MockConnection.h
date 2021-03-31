// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.
//
// This is literally just the EchoConnection, but we can't use the
// EchoConnection because it's in TerminalConnection.dll and loading that in
// these tests is fraught with peril. Easier just to have a local copy.

#pragma once

#include "../cascadia/inc/cppwinrt_utils.h"

namespace ControlUnitTests
{
    class MockConnection : public winrt::implements<MockConnection, winrt::Microsoft::Terminal::TerminalConnection::ITerminalConnection>
    {
    public:
        MockConnection() noexcept = default;

        void Start() noexcept {};
        void WriteInput(winrt::hstring const& data)
        {
            std::wstringstream prettyPrint;
            for (const auto& wch : data)
            {
                if (wch < 0x20)
                {
                    prettyPrint << L"^" << gsl::narrow_cast<wchar_t>(wch + 0x40);
                }
                else if (wch == 0x7f)
                {
                    prettyPrint << L"0x7f";
                }
                else
                {
                    prettyPrint << wch;
                }
            }
            _TerminalOutputHandlers(prettyPrint.str());
        }
        void Resize(uint32_t /*rows*/, uint32_t /*columns*/) noexcept {}
        void Close() noexcept {}

        winrt::Microsoft::Terminal::TerminalConnection::ConnectionState State() const noexcept { return winrt::Microsoft::Terminal::TerminalConnection::ConnectionState::Connected; }

        WINRT_CALLBACK(TerminalOutput, winrt::Microsoft::Terminal::TerminalConnection::TerminalOutputHandler);
        TYPED_EVENT(StateChanged, winrt::Microsoft::Terminal::TerminalConnection::ITerminalConnection, IInspectable);
    };
}
