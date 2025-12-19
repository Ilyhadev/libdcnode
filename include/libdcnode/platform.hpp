#ifndef LIBDCNODE_PLATFORM_HPP_
#define LIBDCNODE_PLATFORM_HPP_

#include <stdexcept>

typedef uint32_t (*PlatformSpecificGetTimeMsFunc)(void);
typedef bool (*PlatformSpecificRequestRestartFunc)(void);
typedef void (*PlatformSpecificReadUniqueIDFunc)(uint8_t out_uid[16]);

typedef int16_t (*CanDriverInitFunc)(uint32_t can_speed, uint8_t can_driver_idx);
typedef int16_t (*CanDriverReceiveFunc)(CanardCANFrame *const rx_frame, uint8_t can_driver_idx);
typedef int16_t (*CanDriverTransmitFunc)(const CanardCANFrame *const tx_frame, uint8_t can_driver_idx);
typedef uint64_t (*CanDriverGetRxOverflowCountFunc)(void);
typedef uint64_t (*CanDriverGetErrorCountFunc)(void);

typedef struct
{
    CanDriverInitFunc init;
    CanDriverReceiveFunc recv;
    CanDriverTransmitFunc send;
    CanDriverGetRxOverflowCountFunc getRxOverflowCount;
    CanDriverGetErrorCountFunc getErrorCount;
} CanDriverApi;

typedef struct
{
    PlatformSpecificGetTimeMsFunc getTimeMs;
    PlatformSpecificRequestRestartFunc requestRestart;
    PlatformSpecificReadUniqueIDFunc readUniqueId;

    CanDriverApi can;
} PlatformApi;

namespace libdcnode
{

    class PlatformManager
    {
    public:
        // Get singleton instance
        static PlatformManager &instance()
        {
            static PlatformManager s_instance;
            return s_instance;
        }

        // Initialize with the platform api
        void init(const PlatformApi &api)
        {
            _platform = &api;
            _initialized = true;
        }

        // Get the platform api (throws if not initialized)
        const PlatformApi &getPlatformApi() const
        {
            assert(_initialized && _platform != nullptr);
            return *_platform;
        }

        bool isInitialized() const
        {
            return _initialized && _platform != nullptr;
        }

        // Reset (for testing)
        void reset()
        {
            _platform = nullptr;
            _initialized = false;
        }

    private:
        // Private constructor for singleton pattern
        PlatformManager() : _platform(nullptr), _initialized(false) {}

        // Prevent copies and moves
        PlatformManager(const PlatformManager &) = delete;
        PlatformManager &operator=(const PlatformManager &) = delete;
        PlatformManager(PlatformManager &&) = delete;
        PlatformManager &operator=(PlatformManager &&) = delete;

        const PlatformApi *_platform = nullptr;
        bool _initialized = false;
    };

    inline const PlatformApi &getPlatformApi()
    {
        return PlatformManager::instance().getPlatformApi();
    }

} // namespace libdcnode

#endif // LIBDCNODE_PLATFORM_HPP_
