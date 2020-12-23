#include <ATen/Functions.h>
#include <ATen/core/TensorBody.h>
#include <algorithm>
#include <array>
#include <bits/c++config.h>
#include <bits/stdint-uintn.h>
#include <c10/core/TensorOptions.h>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <iterator>
#include <limits>
#include <memory>
#include <ostream>
#include <random>
#include <string>
#include <torch/nn/module.h>
#include <torch/nn/modules/activation.h>
#include <torch/nn/modules/container/sequential.h>
#include <torch/nn/modules/conv.h>
#include <torch/nn/modules/linear.h>
#include <torch/nn/options/conv.h>
#include <torch/torch.h>
#include <torch/types.h>
#include <utility>
#include <vector>

constexpr size_t mapWidth = 27;
constexpr size_t mapHeight = 23;
// clang-format off
constexpr int map[mapHeight][mapWidth] = {
 { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1 },
 { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
 { 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1 },
 { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
 { 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1 },
 { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
 { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

// clang-format on

enum class MarineKind { Terminator };
enum class GenestealerKind { Genestealer };

enum class Facing { Left, Right, Up, Down };

enum class TurnPhase {
  CommandPhase,
  SpaceMarineActionPhase,
  ReinforcementPhase,
  StealerActionPhase,
  MarineWon,
  StealerWon,
  EndPhase
};

using CoordinateType = std::uint8_t;
using Vector2 = std::pair<CoordinateType, CoordinateType>;

constexpr bool isPassable(CoordinateType x, CoordinateType y) {
  if (x >= mapWidth or y >= mapHeight) {
    return false;
  }

  return map[y][x];
}

constexpr bool isPassable(Vector2 pos) {
  return isPassable(pos.first, pos.second);
}
constexpr Vector2 targetPos(20, 2);

constexpr std::array<Vector2, 6> SpawnPoints = {
    Vector2(26, 7),  Vector2(26, 13), Vector2(18, 19),
    Vector2(22, 19), Vector2(10, 18), Vector2(10, 22)};

static Vector2 facingToDir(Facing f) {
  switch (f) {
  case Facing::Left:
    return {-1, 0};
  case Facing::Right:
    return {1, 0};
  case Facing::Down:
    return {0, 1};
  case Facing::Up:
    return {0, -1};
  }
  return {0, -1};
}

static Facing turnRight(Facing facing) {
  switch (facing) {
  case Facing::Left:
    return Facing::Up;
  case Facing::Right:
    return Facing::Down;
  case Facing::Down:
    return Facing::Left;
  case Facing::Up:
    return Facing::Right;
  }
  return Facing::Left;
}

static Facing turnLeft(Facing facing) {
  switch (facing) {
  case Facing::Left:
    return Facing::Down;
  case Facing::Right:
    return Facing::Up;
  case Facing::Down:
    return Facing::Right;
  case Facing::Up:
    return Facing::Left;
  }
  return Facing::Left;
}

template <typename T, std::uint8_t startActionPoints,
          std::uint8_t MaxAttackRange = 1>
class Model {
public:
  static constexpr std::uint8_t MaxRange = MaxAttackRange;
  Model() = default;
  explicit Model(T kind, size_t x, size_t y, bool alv = false,
                 Facing fac = Facing::Right)
      : loc(x, y), modelKind(kind), activated(false), alive(alv), f(fac) {}
  T getKind() const { return modelKind; }

  const Vector2 &getLocation() const { return loc; }
  Vector2 &getLocation() { return loc; }

  const CoordinateType &getX() const { return loc.first; }
  const CoordinateType &getY() const { return loc.second; }

  CoordinateType &getX() { return loc.first; }
  CoordinateType &getY() { return loc.second; }

  auto getActionPoints() const { return actionPoints; }
  void setActionPoints(std::uint8_t p) { actionPoints = p; }

  Facing getDirection() const { return f; }
  void setFacing(Facing fac) { f = fac; }

  void moveForward() { loc = positionInFront(); }

  Vector2 positionInDirection(Facing f) const {
    const auto dir = facingToDir(f);
    auto x = getX() + dir.first;
    auto y = getY() + dir.second;
    return {x, y};
  }

  Vector2 positionCornerLeft() const {
    const auto dir = facingToDir(f);
    const auto dir2 = facingToDir(turnLeft(f));
    auto x = getX() + dir.first + dir2.first;
    auto y = getY() + dir.second + dir2.second;
    return {x, y};
  }

  Vector2 positionCornerRight() const {
    const auto dir = facingToDir(f);
    const auto dir2 = facingToDir(turnRight(f));
    auto x = getX() + dir.first + dir2.first;
    auto y = getY() + dir.second + dir2.second;
    return {x, y};
  }

  Vector2 positionInFront() const {
    return positionInDirection(getDirection());
  }

  std::array<Vector2, 3> positionsInFront() const {
    return {positionCornerLeft(), positionInFront(), positionCornerRight()};
  }

  bool hasBeenActivated() const { return activated; }
  void setActivated(bool act) { activated = act; }

  bool isAlive() const { return alive; }

  void setAlive(bool alv) { alive = alv; }

private:
  Vector2 loc{0, 11};
  std::uint8_t actionPoints{startActionPoints};
  T modelKind{T()};
  bool activated{false};
  bool alive{false};
  Facing f{Facing::Left};
};

using Marine = Model<MarineKind, 4, mapWidth>;
using Genestealer = Model<GenestealerKind, 6>;
using Blip = Model<std::uint8_t, 6>;

class Door {
public:
  explicit Door(CoordinateType x, CoordinateType y)
      : opn(false), location(x, y) {}

  bool isOpen() const { return opn; }
  void setOpen(bool open) { opn = open; }
  Vector2 getLocation() const { return location; }
  void toggle() { opn = not opn; }

private:
  bool opn;
  Vector2 location;
};

constexpr std::uint8_t maxStealerCount = 10;
constexpr std::uint8_t totalBlipsCount = 22;
constexpr std::uint8_t reinfocrmentBlips = 2;

class Game {
public:
  Game() = default;
  void setPhase(TurnPhase currentPhase) { phase = currentPhase; }
  TurnPhase getPhase() const { return phase; }

  size_t getTurn() const { return turn; }
  size_t getCommandPoints() const { return commandPoints; }

  std::uint8_t getCurrentActiveModel() const { return currModel; }

  const Genestealer &getCurrentStealer() const {
    assert(isAStealerActive());
    return stelaers[currModel];
  }

  bool isAnyoneActive() const {
    return isABlipActive() or isAMarineActive() or isAStealerActive();
  }

  Genestealer &getCurrentStealer() {
    assert(isAStealerActive());
    return stelaers[currModel];
  }

  const Blip &getCurrentBlip() const {
    assert(isABlipActive());
    return blips[currModel - maxStealerCount];
  }

  Blip &getCurrentBlip() {
    assert(isABlipActive());
    return blips[currModel - maxStealerCount];
  }

  const Marine &getCurrentMarine() const {
    assert(isAMarineActive());
    return marines[currModel];
  }

  Marine &getCurrentMarine() {
    assert(isAMarineActive());
    return marines[currModel];
  }

  void nextPhase() {
    currModel = std::numeric_limits<std::uint8_t>::max();
    if (phase == TurnPhase::SpaceMarineActionPhase) {
      drawnBlips = (drawnBlips + reinfocrmentBlips) % totalBlipsCount;
      phase = TurnPhase::ReinforcementPhase;
      return;
    }

    if (phase == TurnPhase::ReinforcementPhase) {
      phase = TurnPhase::StealerActionPhase;
      return;
    }

    if (phase == TurnPhase::StealerActionPhase) {
      for (auto &m : stelaers) {
        m.setActionPoints(6);
        m.setActivated(false);
      }

      for (auto &m : blips) {
        m.setActionPoints(6);
        m.setActivated(false);
      }
      nextTurn();
      return;
    }
  }

  void nextTurn() {
    for (auto &m : marines) {
      m.setActionPoints(4);
      m.setActivated(false);
    }

    turn++;
    phase = TurnPhase::SpaceMarineActionPhase;
    currModel = std::numeric_limits<std::uint8_t>::max();
  }
  Door *getDoor(Vector2 location) {
    auto maybeDoor =
        std::find_if(doors.begin(), doors.end(), [&location](const Door &door) {
          return location == door.getLocation();
        });
    if (maybeDoor == doors.end())
      return nullptr;

    return &*maybeDoor;
  }

  const Door *getDoor(Vector2 location) const {
    auto maybeDoor =
        std::find_if(doors.begin(), doors.end(), [&location](const Door &door) {
          return location == door.getLocation();
        });
    if (maybeDoor == doors.end())
      return nullptr;

    return &*maybeDoor;
  }

  template <typename Callable>
  auto applyAtModel(std::uint8_t modelId, Callable &&callable) {
    if (phase == TurnPhase::SpaceMarineActionPhase)
      return callable(marines[modelId]);
    assert(phase == TurnPhase::StealerActionPhase);
    if (modelId < maxStealerCount)
      return callable(stelaers[modelId]);
    return callable(blips[modelId - maxStealerCount]);
  }

  template <typename Callable>
  auto applyAtModel(std::uint8_t modelId, Callable &&callable) const {
    if (phase == TurnPhase::SpaceMarineActionPhase)
      return callable(marines[modelId]);
    assert(phase == TurnPhase::StealerActionPhase);
    if (modelId < maxStealerCount)
      return callable(stelaers[modelId]);
    return callable(blips[modelId - maxStealerCount]);
  }

  template <typename Callable> auto applyAtCurrent(Callable &&callable) const {
    return applyAtModel(currModel, std::forward<Callable>(callable));
  }

  template <typename Callable> auto applyAtCurrent(Callable &&callable) {
    return applyAtModel(currModel, std::forward<Callable>(callable));
  }

  void setCurrentModel(std::uint8_t marineId) { currModel = marineId; }

  const Marine &getMarine(std::uint8_t marineId) const {
    return marines[marineId];
  }

  Marine &getMarine(std::uint8_t marineId) { return marines[marineId]; }

  auto &getMarines() { return marines; }
  const auto &getMarines() const { return marines; }

  auto &getStealers() { return stelaers; }
  const auto &getStealers() const { return stelaers; }

  auto &getBlips() { return blips; }
  const auto &getBlips() const { return blips; }

  bool isAMarineActive() const {
    return phase == TurnPhase::SpaceMarineActionPhase and
           currModel < marines.size();
  }

  bool isABlipActive() const {
    return phase == TurnPhase::StealerActionPhase and
           currModel - 10 < aliveBlipCount();
  }

  bool isAStealerActive() const {
    return phase == TurnPhase::StealerActionPhase and
           currModel < aliveStealersCount();
  }

  std::uint8_t aliveStealersCount() const {
    return std::distance(
        stelaers.begin(),
        std::find_if(stelaers.begin(), stelaers.end(),
                     [](const auto &s) { return not s.isAlive(); }));
  }

  std::uint8_t aliveBlipCount() const {
    return std::count_if(blips.begin(), blips.end(),
                         [](const auto &b) { return b.isAlive(); });
  }

  bool isOccupied(Vector2 location) const {
    if (std::any_of(
            marines.begin(), marines.end(), [location](const auto &marine) {
              return marine.isAlive() and marine.getLocation() == location;
            }))
      return true;
    if (std::any_of(
            stelaers.begin(), stelaers.end(), [location](const auto &marine) {
              return marine.isAlive() and marine.getLocation() == location;
            }))
      return true;

    if (std::any_of(blips.begin(), blips.end(), [location](const auto &marine) {
          return marine.isAlive() and marine.getLocation() == location;
        }))
      return true;

    if (std::any_of(doors.begin(), doors.end(), [location](const auto &marine) {
          return marine.getLocation() == location and not marine.isOpen();
        }))
      return true;

    return false;
  }

  std::uint8_t getCurrentActionPoints() const {
    assert(phase == TurnPhase::SpaceMarineActionPhase or
           phase == TurnPhase::StealerActionPhase);
    if (isAMarineActive())
      return getCurrentMarine().getActionPoints();
    if (isAStealerActive())
      return getCurrentStealer().getActionPoints();
    if (isABlipActive())
      return getCurrentBlip().getActionPoints();
    return 0;
  }

  bool hasBlipsToPlace() const {
    return phase == TurnPhase::ReinforcementPhase and drawnBlips != toUseBlips;
  }

  std::size_t blipsAtLocation(const Vector2 &loc) const {
    return std::count_if(blips.begin(), blips.end(), [&loc](const Blip &b) {
      return b.isAlive() and b.getLocation() == loc;
    });
  }

  void spawnNextBlipAt(Vector2 location) {
    assert(hasBlipsToPlace());
    auto blipValue = blipsStack[toUseBlips++];
    auto toReplace =
        std::find_if(blips.begin(), blips.end(),
                     [](const Blip &blip) { return not blip.isAlive(); });

    assert(toReplace != blips.end());
    *toReplace = Blip(blipValue, location.first, location.second, true);

    std::stable_partition(blips.begin(), blips.end(),
                          [](const auto &blip) { return blip.isAlive(); });
  }

  std::size_t countBlisps() const {
    return std::count_if(blips.begin(), blips.end(),
                         [](const Blip &blip) { return blip.isAlive(); });
  }

  void dumpSquare(Vector2 loc, std::ostream &OS) const {
    for (const auto &spawn : SpawnPoints) {
      if (spawn == loc) {
        OS << 'S';
        return;
      }
    }

    for (const auto &door : doors) {
      if (door.getLocation() == loc and not door.isOpen()) {
        OS << 'D';
        return;
      }
    }

    for (const auto &marine : marines) {
      if (marine.getLocation() == loc and marine.isAlive()) {
        OS << 'M';
        return;
      }
    }

    for (const auto &blip : blips) {
      if (blip.getLocation() == loc and blip.isAlive()) {
        OS << 'B';
        return;
      }
    }

    if (not isPassable(loc)) {
      OS << 'X';
      return;
    }

    OS << ' ';
  }

  void dump(std::ostream &OS) const {
    for (size_t y = 0; y < mapHeight; y++) {
      for (size_t x = 0; x < mapWidth; x++) {
        dumpSquare(Vector2(x, y), OS);
      }
      OS << '\n';
    }
  }

  void afterKill(bool marineWasKiller) {
    if (marineWasKiller) {
      std::stable_partition(blips.begin(), blips.end(),
                            [](const auto &blip) { return blip.isAlive(); });
      marineKills++;

    } else {
      std::stable_partition(marines.begin(), marines.end(),
                            [](const auto &blip) { return blip.isAlive(); });
      blipsKill++;
    }
  }

  size_t getMarineKills() const { return marineKills; }
  size_t getStealerKills() const { return blipsKill; }

  torch::Tensor toTensor() const {
    return (torch::from_blob((void *)this, {1, sizeof(Game)},
                             torch::TensorOptions(torch::kUInt8))
                .to(torch::kFloat32) /
            std::numeric_limits<uint8_t>::max());
  }

private:
  TurnPhase phase{TurnPhase::CommandPhase};
  std::uint8_t commandPoints{0};
  std::uint8_t turn{0};
  std::array<Marine, 5> marines{Marine(MarineKind::Terminator, 0, 11, true),
                                Marine(MarineKind::Terminator, 1, 11, true),
                                Marine(MarineKind::Terminator, 2, 11, true),
                                Marine(MarineKind::Terminator, 3, 11, true),
                                Marine(MarineKind::Terminator, 4, 11, true)};
  std::array<Genestealer, maxStealerCount> stelaers;
  std::array<Blip, maxStealerCount> blips;
  std::array<Door, 7> doors{Door(5, 11), Door(9, 11), Door(7, 13), Door(7, 18),
                            Door(9, 20), Door(15, 9), Door(20, 4)};
  std::uint8_t currModel{6};

  std::uint8_t drawnBlips{0};
  std::uint8_t toUseBlips{0};
  std::array<uint8_t, 22> blipsStack{1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2,
                                     2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3};

  size_t marineKills{0};
  size_t blipsKill{0};
};

static bool canMoveForward(const Game &g) {
  if (not g.isAnyoneActive())
    return false;

  const auto act = [&g](const auto &model) {
    if (model.getActionPoints() == 0)
      return false;

    auto pos = model.positionInFront();
    bool passable = isPassable(pos) and not g.isOccupied(pos);

    return passable;
  };

  return g.applyAtCurrent(act);
}

static void actionMoveForward(Game &g) {
  assert(canMoveForward(g));

  const auto act = [&g](auto &model) {
    model.setActionPoints(model.getActionPoints() - 1);
    model.moveForward();

    if (model.getLocation() == targetPos)
      g.setPhase(TurnPhase::MarineWon);
  };

  g.applyAtCurrent(act);
}

static bool canTurn(const Game &g) {
  if (not g.isAnyoneActive())
    return false;

  return g.getCurrentActionPoints() > 0;
}

template <bool Left> static void actionTurn(Game &g) {
  assert(canTurn(g));
  g.applyAtCurrent([](auto &model) {
    model.setActionPoints(model.getActionPoints() - 1);
    if (Left)
      model.setFacing(turnLeft(model.getDirection()));
    else
      model.setFacing(turnRight(model.getDirection()));
  });
}

static double distanceFromEnd(const Game &g) {
  double averageDistance = 0.0;
  for (const auto &marine : g.getMarines()) {
    auto marineLocation = marine.getLocation();
    auto xDistance =
        std::abs(double(marineLocation.first) - double(targetPos.first));
    auto yDistance =
        std::abs(double(marineLocation.second) - double(targetPos.second));
    auto increment = (xDistance + yDistance) / (mapWidth + mapHeight);
    assert(increment < 1 and increment > 0);
    averageDistance += increment;
  }
  averageDistance /= g.getMarines().size();

  auto val = averageDistance;
  assert(val < 1 and val > 0);
  return val;
}

static double averageDistanceFromMarine(const Game &g) {
  if (g.aliveBlipCount() == 0)
    return 1;

  double minDistance = 0.0;
  for (const auto &s : g.getBlips()) {
    if (not s.isAlive())
      continue;

    auto stealer = s.getLocation();
    auto marine = g.getMarines()[0].getLocation();
    auto xDistance = std::abs(double(stealer.first) - double(marine.first));
    auto yDistance = std::abs(double(stealer.second) - double(marine.second));
    // add one for each blip near, 0 for each blip faraway
    minDistance += ((xDistance + yDistance) / (mapHeight + mapWidth));
  }

  minDistance /= g.aliveBlipCount();

  assert(minDistance <= 1 and minDistance >= 0);

  return minDistance;
}

static double marineScoring(const Game &g) {
  if (g.getPhase() == TurnPhase::StealerWon) {
    // std::cout << "Stealer won\n";
    return 0;
  }

  if (g.getPhase() == TurnPhase::MarineWon) {
    // std::cout << "Marine won\n";
    return 1;
  }

  if (g.getStealerKills() > 3)
    return 0;

  if (g.getMarineKills() > 10)
    return 1;

  double distanceFactor = distanceFromEnd(g);
  double blipsDistanceFactor = averageDistanceFromMarine(g);

  // std::cout << "marine closeness " << 1 - distanceFactor << "\n";

  return (1 - distanceFactor) * blipsDistanceFactor;
}

template <typename ModelIterator>
bool allActivated(ModelIterator b, ModelIterator e) {
  return std::all_of(b, e, [](const auto &m) { return m.hasBeenActivated(); });
}

template <typename ModelIteratorRange> bool allActivated(ModelIteratorRange c) {
  auto b = std::begin(c);
  auto e = std::find_if(b, c.end(),
                        [](const auto &model) { return not model.isAlive(); });
  return std::all_of(b, e, [](const auto &m) { return m.hasBeenActivated(); });
}

static bool canPassTrun(const Game &g) {
  if (g.getPhase() == TurnPhase::SpaceMarineActionPhase) {
    return allActivated(g.getMarines());
  }

  if (g.hasBlipsToPlace() and g.countBlisps() < maxStealerCount) {
    return false;
  }

  if (g.getPhase() == TurnPhase::StealerActionPhase) {
    return allActivated(g.getStealers()) and allActivated(g.getBlips());
  }

  return true;
}

static void passTurn(Game &g) {
  assert(canPassTrun(g));
  g.nextPhase();
}

static bool canToggleDoor(const Game &g) {
  if (not g.isAnyoneActive())
    return false;

  const auto &canOpenDoorImpl = [&g](const auto &model) {
    if (model.getActionPoints() == 0)
      return false;

    auto frontPos = model.positionsInFront();
    return std::any_of(frontPos.begin(), frontPos.end(),
                       [&g](const auto &p) { return g.getDoor(p) != nullptr; });
  };

  return g.applyAtCurrent(canOpenDoorImpl);
}

static void actionToggleDoor(Game &g) {
  assert(canToggleDoor(g));

  const auto opnDoor = [&g](auto &model) {
    model.setActionPoints(model.getActionPoints() - 1);
    auto frontPos = model.positionsInFront();

    for (auto p : frontPos) {
      auto *door = g.getDoor(p);
      if (door == nullptr)
        continue;

      door->toggle();
    }
  };

  g.applyAtCurrent(opnDoor);
}

template <std::uint8_t modelId> static bool canActivate(const Game &g) {
  if (g.getPhase() != TurnPhase::SpaceMarineActionPhase and
      g.getPhase() != TurnPhase::StealerActionPhase)
    return false;

  if (g.getCurrentActiveModel() == modelId)
    return false;

  if (g.getPhase() == TurnPhase::StealerActionPhase and g.isAStealerActive())
    if (g.getCurrentStealer().getActionPoints() > 0)
      return false;

  return g.applyAtModel(modelId, [](const auto &m) {
    return not m.hasBeenActivated() and m.isAlive();
  });
}

template <std::uint8_t modelId> static void actionMarineActivate(Game &g) {
  assert(canActivate<modelId>(g));

  g.setCurrentModel(modelId);
  g.applyAtModel(modelId, [](auto &m) { m.setActivated(true); });
}

template <std::uint8_t blipLocation> bool canPlaceBlip(const Game &g) {
  return g.hasBlipsToPlace() and
         g.blipsAtLocation(SpawnPoints[blipLocation]) < 3 and
         g.countBlisps() < maxStealerCount;
}

template <std::uint8_t blipLocation> void actionPlaceBlip(Game &g) {
  assert(canPlaceBlip<blipLocation>(g));
  g.spawnNextBlipAt(SpawnPoints[blipLocation]);
}

template <typename Model, typename Candidates>
decltype(&Candidates{}[0]) getFacingModel(const Game &g, Model &model,
                                          Candidates &candidates) {
  auto MaxRange = model.MaxRange;
  Vector2 startingLoc = model.getLocation();
  Vector2 direction = facingToDir(model.getDirection());

  for (int a = 1; a <= MaxRange; a++) {
    Vector2 targetLoc(startingLoc.first + direction.first,
                      startingLoc.second + direction.second);
    if (not isPassable(targetLoc))
      return nullptr;
    auto *door = g.getDoor(targetLoc);
    if (door and not door->isOpen())
      return nullptr;

    auto model = std::find_if(
        candidates.begin(), candidates.end(), [&targetLoc](const auto &target) {
          return target.getLocation() == targetLoc and target.isAlive();
        });

    if (model != candidates.end())
      return &(*model);
  }

  return nullptr;
}

static bool canFight(const Game &g) {
  if (g.getPhase() != TurnPhase::SpaceMarineActionPhase and
      g.getPhase() != TurnPhase::StealerActionPhase)
    return false;

  if (not g.isAnyoneActive())
    return false;

  if (g.getCurrentActionPoints() == 0)
    return false;

  if (g.getPhase() == TurnPhase::SpaceMarineActionPhase) {
    return nullptr != getFacingModel(g, g.getCurrentMarine(), g.getBlips());
  }

  return nullptr != getFacingModel(g, g.getCurrentBlip(), g.getMarines());
}

static void actionFight(Game &g) {
  assert(canFight(g));
  if (g.getPhase() == TurnPhase::SpaceMarineActionPhase) {
    auto target = getFacingModel(g, g.getCurrentMarine(), g.getBlips());
    target->setAlive(false);
    g.afterKill(true);
    return;
  }

  auto target = getFacingModel(g, g.getCurrentBlip(), g.getMarines());
  target->setAlive(false);
  if (not g.getMarines()[0].isAlive())
    g.setPhase(TurnPhase::StealerWon);
  g.afterKill(false);
  return;
}

using Action = void (*)(Game &);
using CanPerformAction = bool (*)(const Game &);
using ActionPair = std::pair<CanPerformAction, Action>;

constexpr size_t ActionsCount = 25;
static constexpr std::array<ActionPair, ActionsCount> Actions = {
    std::make_pair(canMoveForward, actionMoveForward),
    std::make_pair(canTurn, actionTurn<true>),
    std::make_pair(canPassTrun, passTurn),
    std::make_pair(canTurn, actionTurn<false>),
    std::make_pair(canToggleDoor, actionToggleDoor),
    std::make_pair(canActivate<0>, actionMarineActivate<0>),
    std::make_pair(canActivate<1>, actionMarineActivate<1>),
    std::make_pair(canActivate<2>, actionMarineActivate<2>),
    std::make_pair(canActivate<3>, actionMarineActivate<3>),
    std::make_pair(canActivate<4>, actionMarineActivate<4>),

    std::make_pair(canActivate<maxStealerCount + 0>,
                   actionMarineActivate<maxStealerCount + 0>),
    std::make_pair(canActivate<maxStealerCount + 1>,
                   actionMarineActivate<maxStealerCount + 1>),
    std::make_pair(canActivate<maxStealerCount + 2>,
                   actionMarineActivate<maxStealerCount + 2>),
    std::make_pair(canActivate<maxStealerCount + 3>,
                   actionMarineActivate<maxStealerCount + 3>),
    std::make_pair(canActivate<maxStealerCount + 4>,
                   actionMarineActivate<maxStealerCount + 4>),
    std::make_pair(canActivate<maxStealerCount + 5>,
                   actionMarineActivate<maxStealerCount + 5>),
    std::make_pair(canActivate<maxStealerCount + 6>,
                   actionMarineActivate<maxStealerCount + 6>),
    std::make_pair(canActivate<maxStealerCount + 7>,
                   actionMarineActivate<maxStealerCount + 7>),
    std::make_pair(canActivate<maxStealerCount + 8>,
                   actionMarineActivate<maxStealerCount + 8>),
    std::make_pair(canActivate<maxStealerCount + 9>,
                   actionMarineActivate<maxStealerCount + 9>),
    std::make_pair(canFight, actionFight),
    std::make_pair(canPlaceBlip<0>, actionPlaceBlip<0>),
    std::make_pair(canPlaceBlip<1>, actionPlaceBlip<1>),
    std::make_pair(canPlaceBlip<2>, actionPlaceBlip<2>),
    std::make_pair(canPlaceBlip<3>, actionPlaceBlip<3>)};
// std::make_pair(canPlaceBlip<5>, actionPlaceBlip<5>)};

static std::string actionToString(Action act) {
  if (act == actionMoveForward)
    return "move forward";
  if (act == actionTurn<true>)
    return "turn left";
  if (act == actionTurn<false>)
    return "turn right";
  if (act == passTurn)
    return "pass turn";
  if (act == actionToggleDoor)
    return "toggle door";
  if (act == actionMarineActivate<0>)
    return "activate marine 0";
  if (act == actionMarineActivate<1>)
    return "activate marine 1";
  if (act == actionMarineActivate<2>)
    return "activate marine 2";
  if (act == actionMarineActivate<3>)
    return "activate marine 3";
  if (act == actionMarineActivate<4>)
    return "activate marine 4";

  if (act == actionMarineActivate<maxStealerCount + 0>)
    return "activate blip 0";
  if (act == actionMarineActivate<maxStealerCount + 1>)
    return "activate blip 1";
  if (act == actionMarineActivate<maxStealerCount + 2>)
    return "activate blip 2";
  if (act == actionMarineActivate<maxStealerCount + 3>)
    return "activate blip 3";
  if (act == actionMarineActivate<maxStealerCount + 4>)
    return "activate blip 4";

  if (act == actionMarineActivate<maxStealerCount + 5>)
    return "activate blip 5";
  if (act == actionMarineActivate<maxStealerCount + 6>)
    return "activate blip 6";
  if (act == actionMarineActivate<maxStealerCount + 7>)
    return "activate blip 7";
  if (act == actionMarineActivate<maxStealerCount + 8>)
    return "activate blip 8";
  if (act == actionMarineActivate<maxStealerCount + 9>)
    return "activate blip 9";
  if (act == actionPlaceBlip<0>)
    return "place blip at 0";
  if (act == actionPlaceBlip<1>)
    return "place blip at 1";
  if (act == actionPlaceBlip<2>)
    return "place blip at 2";
  if (act == actionPlaceBlip<3>)
    return "place blip at 3";
  if (act == actionPlaceBlip<4>)
    return "place blip at 4";
  if (act == actionPlaceBlip<5>)
    return "place blip at 5";
  if (act == actionFight)
    return "fight";

  return "Error";
}

class MonteCarloNode {
public:
  template <typename Net, typename... Args>
  explicit MonteCarloNode(Action action, MonteCarloNode *parent, Net &net,
                          Args &&... args)
      : action(action), parent(parent), g(std::forward<Args>(args)...),
        extimatedReward(0) {
    if (action != nullptr)
      action(getGame());

    auto p = net.forward(g.toTensor());
    auto acc = p.template accessor<float, 2>();

    for (size_t i = 0; i < ActionsCount; i++) {
      possibleActions[i] = std::make_pair(i, Actions[i]);
    }

    std::sort(possibleActions.begin(), possibleActions.end(),
              [&acc](const auto &l, const auto &r) {
                return acc[0][l.first] < acc[0][r.first];
              });

    auto it = std::remove_if(possibleActions.begin(), possibleActions.end(),
                             [this](const auto &indexedAction) {
                               return not indexedAction.second.first(g);
                             });

    actionsCount = std::distance(possibleActions.begin(), it);

    assert(actionsCount > 0 or isTerminal());
  }

  using content = std::vector<std::unique_ptr<MonteCarloNode>>;
  using iterator = content::iterator;
  using const_iterator = content::const_iterator;

  iterator begin() { return std::begin(childres); }
  iterator end() { return std::end(childres); }

  const_iterator begin() const { return std::begin(childres); }
  const_iterator end() const { return std::end(childres); }
  size_t childCounts() const { return childres.size(); }

  template <typename Net>
  MonteCarloNode &performAction(Action action, Net &net) {
    auto p(std::make_unique<MonteCarloNode>(action, this, net, g));
    childres.emplace_back(move(p));
    return *childres.back();
  }

  bool isNodeMarine() const {
    return getGame().getPhase() != TurnPhase::StealerActionPhase and
           getGame().getPhase() != TurnPhase::ReinforcementPhase;
  }

  size_t possibleActionsCount() const {
    return actionsCount;
    auto num = std::count_if(Actions.begin(), Actions.end(),
                             [this](auto f) { return f.first(g); });
    assert(num != 0);
    return num;
  }

  bool isFullyExpanded() const { return childCounts() == actionsCount; }

  template <typename Net> MonteCarloNode &expand(Net &net) {
    assert(not isFullyExpanded());
    return performAction(possibleActions[childCounts()].second.second, net);
  }

  size_t visitsCount() const { return visits; }

  double marineWinRatio() const {
    return (double(extimatedReward) / double(visitsCount()));
  }

  double getExtimatedReward(bool marineReward) const {
    return marineReward ? marineWinRatio() : 1.0 - marineWinRatio();
  }

  double ubc(const MonteCarloNode &child) const {
    return (child.getExtimatedReward(isNodeMarine())) +
           (0.1 * std::sqrt(2 * std::log(visitsCount()) / child.visitsCount()));
  }

  MonteCarloNode &bestChild() {
    return **std::max_element(begin(), end(),
                              [this](const auto &lChild, const auto &rChild) {
                                return ubc(*lChild) < ubc(*rChild);
                              });
  }

  const MonteCarloNode &bestChild() const {
    return **std::max_element(begin(), end(),
                              [this](const auto &lChild, const auto &rChild) {
                                return ubc(*lChild) < ubc(*rChild);
                              });
  }

  void addReward(double reward) { extimatedReward += reward; }

  void visit() { visits++; }
  MonteCarloNode *getParent() { return parent; }
  const MonteCarloNode *getParent() const { return parent; }

  bool isTerminal() {
    if (g.getTurn() > 15)
      return true;
    return g.getPhase() == TurnPhase::MarineWon or
           g.getPhase() == TurnPhase::StealerWon;
  }

  size_t dumpGraphviz(std::ostream &OS, size_t id) {
    OS << "\"" << id << "\" [label=\"W: " << getExtimatedReward(isNodeMarine())
       << " R: " << extimatedReward << " V: " << visits << "\"]"
       << "\n";

    size_t nextId = id;
    for (auto &c : *this) {
      OS << "\"" << id << "\" -> \"" << nextId + 1 << "\"[label=\""
         << actionToString(c->getAction()) << "\"]\n";
      nextId = c->dumpGraphviz(OS, nextId + 1);
    }

    return nextId;
  }

  Game &getGame() { return g; }
  const Game &getGame() const { return g; }
  Action getAction() const { return action; }

  template <typename Net> void train(Net &net) const {

    auto result = net.forward(g.toTensor());
    auto acc = result.template accessor<float, 2>();

    std::array<std::pair<std::int32_t, float>, ActionsCount> scores;
    for (size_t i = 0; i < ActionsCount; i++) {
      auto score = i < childres.size() ? childres[0]->marineWinRatio()
                                       : acc[0][possibleActions[i].first];
      scores[i] = std::make_pair(possibleActions[i].first, score);
    }

    sort(scores.begin(), scores.end(),
         [](const auto &l, const auto &r) { return l.first < r.first; });

    auto target =
        torch::from_blob((void *)&(scores[0].second), {1, ActionsCount}, {0, 2},
                         torch::TensorOptions(torch::kFloat32));

    torch::Tensor error = at::mse_loss(result, target);

    error.backward();
    std::cout << error << "\n";
  }

private:
  Action action;
  MonteCarloNode *parent;
  Game g;
  content childres;
  size_t visits{0};
  double extimatedReward;

  std::array<std::pair<size_t, ActionPair>, ActionsCount> possibleActions;
  size_t actionsCount;
};

class MonteCarloTree {
public:
  template <typename Net, typename... Args>
  explicit MonteCarloTree(Net &net, Args &&... args)
      : root(nullptr, nullptr, net, std::forward<Args>(args)...) {
    assert(not root.isTerminal());
  }

  void backup(MonteCarloNode &node, double marineWon) {
    auto *currNode = &node;
    while (currNode != nullptr) {
      currNode->visit();

      currNode->addReward(marineWon);

      auto currentParent = currNode->getParent();
      currNode = currentParent;
    }
  }

  template <typename Net> MonteCarloNode &treePolicy(Net &net) {
    MonteCarloNode *node(&root);
    while (not node->isTerminal()) {
      if (not node->isFullyExpanded())
        return node->expand(net);

      node = &(node->bestChild());
    }
    return *node;
  }

  Action getBestAction() const {
    if (root.isNodeMarine())
      return (*std::max_element(root.begin(), root.end(),
                                [](const auto &l, const auto &r) {
                                  return l->marineWinRatio() <
                                         r->marineWinRatio();
                                }))
          ->getAction();
    return (*std::min_element(root.begin(), root.end(),
                              [](const auto &l, const auto &r) {
                                return l->marineWinRatio() <
                                       r->marineWinRatio();
                              }))
        ->getAction();
  }
  void dumpGraphviz(std::ostream &OS) {
    OS << "digraph g{\n";
    root.dumpGraphviz(OS, 0);
    OS << "}";
  }

  template <typename Net> void train(Net &net) { root.train(net); }

  template <typename Net>
  MonteCarloNode &defaultPolicy(MonteCarloNode *node, Net &net) {
    while (not node->isTerminal())
      node = &node->expand(net);
    return *node;
  }

private:
  MonteCarloNode root;
};

template <typename Network, typename Opt>
Action mcts(const Game &g, Network &net, Opt &optim, size_t maxIter = 1000,
            bool dump = false, bool train = false) {
  MonteCarloTree t(net, g);
  for (size_t i = 0; i < maxIter; i++) {
    auto &node = t.treePolicy(net);
    // auto &terminalNode = t.defaultPolicy(&node, net);
    t.backup(node, marineScoring(node.getGame()));
  }
  if (dump)
    t.dumpGraphviz(std::cout);
  if (train) {
    net.zero_grad();
    t.train(net);
    optim.step();
  }
  return t.getBestAction();
}

template <typename Net>
static void run(size_t turns, size_t iterations, Net &net) {
  Game g;
  torch::optim::Adam generator_optimizer(net.parameters(),
                                         torch::optim::AdamOptions(2e-4));
  g.nextTurn();
  std::cout << "initial scoring: " << marineScoring(g) << "\n";
  for (int a = 0; a < turns; a++) {
    auto f = mcts(g, net, generator_optimizer, iterations, false, true);
    f(g);
    std::cout << actionToString(f) << "\n";
    if (g.getPhase() == TurnPhase::StealerWon or
        g.getPhase() == TurnPhase::MarineWon)
      break;
    for (const auto &m : g.getMarines()) {
      std::cout << (size_t)m.getX() << ", " << (size_t)m.getY() << "\n";
    }
  }

  // auto f = mcts(g, atoi(argv[2]), true);
  g.dump(std::cout);
  std::cout << "final scoring: " << marineScoring(g) << "\n";
}

using namespace torch;
using namespace nn;

int main(int argc, char *argv[]) {
  //
  //
  // Net net(4, 5);
  //
  //
  auto net = Sequential(Linear(sizeof(Game), sizeof(Game)), ReLU(),
                        Linear(sizeof(Game), ActionsCount), ReLU());
  // for (const auto &p : net->parameters()) {
  // std::cout << p << std::endl;
  //}

  for (int a = 0; a < atoi(argv[2]); a++) {
    run(atoi(argv[1]), atoi(argv[2]), *net);
  }
}
