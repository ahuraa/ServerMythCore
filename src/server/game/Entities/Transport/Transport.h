/*
 * Copyright (C) 2008 - 2011 Trinity <http://www.trinitycore.org/>
 *
 * Copyright (C) 2010 - 2013 Myth Project <http://mythprojectnetwork.blogspot.com/>
 *
 * Myth Project's source is based on the Trinity Project source, you can find the
 * link to that easily in Trinity Copyrights. Myth Project is a private community.
 * To get access, you either have to donate or pass a developer test.
 * You may not share Myth Project's sources! For personal use only.
 */

#ifndef TRANSPORTS_H
#define TRANSPORTS_H

#include "GameObject.h"

#include <map>
#include <set>
#include <string>

class Transport : public GameObject
{
    public:
        Transport(uint32 period, uint32 script);
        ~Transport();

        bool Create(uint32 guidlow, uint32 entry, uint32 mapid, float x, float y, float z, float ang, uint32 animprogress, uint32 dynflags);
        bool GenerateWaypoints(uint32 pathid, std::set<uint32> &mapids);
        void Update(uint32 p_time);
        bool AddPassenger(Player* passenger);
        bool RemovePassenger(Player* passenger);

        void RemovePassenger(Creature* passenger) { m_NPCPassengerSet.erase(passenger); }

        typedef std::set<Player*> PlayerSet;
        PlayerSet const& GetPassengers() const { return m_passengers; }

        typedef std::set<Creature*> CreatureSet;
        CreatureSet m_NPCPassengerSet;
        Creature* AddNPCPassenger(uint32 tguid, uint32 entry, float x, float y, float z, float o, uint32 anim=0);
        Creature* AddNPCPassengerInInstance(uint32 entry, float x, float y, float z, float o, uint32 anim=0);
        void UpdatePosition(MovementInfo *mi);
        void UpdateNPCPositions();
        void UpdatePlayerPositions();
        void BuildStartMovePacket(Map const *targetMap);
        void BuildWaitMovePacket(Map const* targermap);
        void BuildStopMovePacket(Map const *targetMap);
        uint32 GetScriptId() const { return ScriptId; }
    private:
        struct WayPoint
        {
            WayPoint() : mapid(0), x(0), y(0), z(0), teleport(false), id(0) { }
            WayPoint(uint32 _mapid, float _x, float _y, float _z, bool _teleport, uint32 _id = 0,
                uint32 _arrivalEventID = 0, uint32 _departureEventID = 0)
                : mapid(_mapid), x(_x), y(_y), z(_z), teleport(_teleport), id(_id),
                arrivalEventID(_arrivalEventID), departureEventID(_departureEventID) { }
            uint32 mapid;
            float x;
            float y;
            float z;
            bool teleport;
            uint32 id;
            uint32 arrivalEventID;
            uint32 departureEventID;
        };

        typedef std::map<uint32, WayPoint> WayPointMap;

        WayPointMap::const_iterator m_curr;
        WayPointMap::const_iterator m_next;
        uint32 m_pathTime;
        uint32 m_timer;

        PlayerSet m_passengers;

        uint32 currenttguid;
        uint32 m_period;
        uint32 ScriptId;
    public:
        WayPointMap m_WayPoints;
        uint32 m_nextNodeTime;

    private:
        void TeleportTransport(uint32 newMapid, float x, float y, float z);
        void UpdateForMap(Map const* map);
        void DoEventIfAny(WayPointMap::value_type const& node, bool departure);
        WayPointMap::const_iterator GetNextWayPoint();
};

#endif

