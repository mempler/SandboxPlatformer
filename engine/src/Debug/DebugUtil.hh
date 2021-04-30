#pragma once

class Engine;
class BaseDebugUtil {
public:
    BaseDebugUtil(Engine *pEngine) : m_pEngine(pEngine) {
    }

protected:
    Engine *m_pEngine;
};
