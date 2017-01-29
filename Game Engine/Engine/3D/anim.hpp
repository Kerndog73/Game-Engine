//
//  anim.hpp
//  Game Engine
//
//  Created by Indi Kernick on 20/1/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef engine_3d_anim_hpp
#define engine_3d_anim_hpp

#include <cstdint>
#include <cassert>
#include <limits>
#include "../Resource/Handles/opengl mesh.hpp"
#include "../Application/global resource cache.hpp"
#include "../Math/interpolate.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace Graphics3D {
  class Anim {
  public:
    using ID = Res::MeshOpenGL::AnimationID;
    
    static const uint64_t DEFAULT_TRANSITION = 100;
    
    Anim(Res::ID meshID);
    
    //the interface is in milliseconds but
    //the implementation is in ticks
    void update(uint64_t);
    const std::vector<glm::mat4> &getBoneTransforms() const;
    
    void play(ID id, uint64_t start = 0, uint64_t transition = DEFAULT_TRANSITION);
    void pause();
    void resume();
    void togglePause();
    
    void setNext(ID id);
    void cancelNext();
    
    void loop(bool = true);
    void loopNext(bool = true);
    
    void setSpeed(double);
  
  private:
    //ticks
    using Time = Res::MeshOpenGL::Time;
    
    enum State {
      RUNNING,
      PAUSED,
      STOPPED,
      TRANSITIONING,
    };
    State state = STOPPED;
    static const ID NONE = std::numeric_limits<ID>::max();
    ID current = NONE;
    ID next = NONE;
    bool willLoop = false;
    bool willLoopNext = false;
    double speed = 1.0;
    Res::ID meshID;
    std::vector<glm::mat4> boneTransforms;
    //progress of the animation. If state is transitioning then this is
    //the progress of the next animation to transition into
    Time progress = 0;
    Time duration = 0;
    //progress of the transition
    Time transitionProgress = 0;
    //duration of the transition
    Time transitionDuration = 0;
    
    Time nextProgress = 0;
    Time nextDuration = 0;
    
    bool pausedTransition = false;
    double ticksPerMillisecond = 0.0;
    
    Res::MeshOpenGL::Ptr getMesh() const;
    bool valid(ID) const;
    bool handleEndRun();
    bool handleEndTrans();
    void setTicksPerMillisecond();
    Time millisToTicks(uint64_t) const;
    uint64_t ticksToMillis(Time) const;
    Time getDuration(ID);
    
    template <typename T>
    T getKey(
      const Res::MeshOpenGL::SubChannel<T> &,
      Time
    ) const;
    
    glm::mat4 getKeyTransform(const Res::MeshOpenGL::Channel &) const;
    glm::mat4 getKeyTransform(
      const Res::MeshOpenGL::Channel &,
      const Res::MeshOpenGL::Channel &
    ) const;
    std::vector<glm::mat4> getBoneNodeTransforms(
      const Res::MeshOpenGL::BoneNodes &,
      const Res::MeshOpenGL::Animation &
    ) const;
    std::vector<glm::mat4> getBoneNodeTransforms(
      const Res::MeshOpenGL::BoneNodes &,
      const Res::MeshOpenGL::Animation &,
      const Res::MeshOpenGL::Animation &
    ) const;
    
    void relativeTransforms(
      std::vector<glm::mat4> &transforms,
      const Res::MeshOpenGL::BoneNodes &boneNodes,
      Res::MeshOpenGL::ChannelID root = 0
    ) const;
    
    void finalTransform(const std::vector<glm::mat4> &,
                        const Res::MeshOpenGL::Bones &);
    
    void doRunning();
    void doTransitioning();
  };
}

extern Graphics3D::Anim *anim;

#endif
