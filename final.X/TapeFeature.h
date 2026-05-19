#ifndef TAPE_FEATURE_H
#define TAPE_FEATURE_H

#include <stdbool.h>
#include <stdint.h>

#ifndef TAPE_F_L_BIT
#define TAPE_F_L_BIT     0x01u
#endif
#ifndef TAPE_F_LM_BIT
#define TAPE_F_LM_BIT    0x02u
#endif
#ifndef TAPE_F_M_BIT
#define TAPE_F_M_BIT     0x04u
#endif
#ifndef TAPE_F_RM_BIT
#define TAPE_F_RM_BIT    0x08u
#endif
#ifndef TAPE_F_R_BIT
#define TAPE_F_R_BIT     0x10u
#endif
#ifndef TAPE_R_LM_BIT
#define TAPE_R_LM_BIT    0x01u
#endif
#ifndef TAPE_R_RM_BIT
#define TAPE_R_RM_BIT    0x02u
#endif

#ifndef FRONT_WIDE_MIN_COUNT
#define FRONT_WIDE_MIN_COUNT 3u
#endif
#ifndef EDGE_LOCK_DEBOUNCE_MS
#define EDGE_LOCK_DEBOUNCE_MS 100u
#endif
#ifndef EDGE_LOST_GRACE_MS
#define EDGE_LOST_GRACE_MS 300u
#endif

typedef struct {
    uint8_t frontMask;
    uint8_t rearMask;

    bool leftEdgeLocked;
    bool rightEdgeLocked;

    bool leftEdgeSoftLost;
    bool rightEdgeSoftLost;

    bool leftBoundaryDanger;
    bool rightBoundaryDanger;

    bool frontWideBlack;
    bool frontAllBlack;
    bool rearAnyBlack;
    bool rearWideBlack;

    int lateralError;
    int headingError;
    int confidence;
} TapeFeature_t;

typedef struct {
    bool leftCandidateActive;
    bool rightCandidateActive;
    bool leftEverLocked;
    bool rightEverLocked;
    bool leftLostActive;
    bool rightLostActive;
    uint32_t leftCandidateSince;
    uint32_t rightCandidateSince;
    uint32_t leftLostSince;
    uint32_t rightLostSince;
} TapeFeatureEstimator_t;

static inline uint8_t TapeFeature_CountBits(uint8_t mask)
{
    uint8_t count = 0;

    while (mask != 0u) {
        if ((mask & 0x01u) != 0u) {
            count++;
        }
        mask >>= 1;
    }

    return count;
}

static inline void TapeFeature_Reset(TapeFeatureEstimator_t *estimator)
{
    estimator->leftCandidateActive = false;
    estimator->rightCandidateActive = false;
    estimator->leftEverLocked = false;
    estimator->rightEverLocked = false;
    estimator->leftLostActive = false;
    estimator->rightLostActive = false;
    estimator->leftCandidateSince = 0u;
    estimator->rightCandidateSince = 0u;
    estimator->leftLostSince = 0u;
    estimator->rightLostSince = 0u;
}

static inline bool TapeFeature_LeftLockCandidate(uint8_t frontMask,
                                                 uint8_t rearMask)
{
    return (((frontMask & TAPE_F_LM_BIT) != 0u) &&
            ((rearMask & TAPE_R_LM_BIT) != 0u) &&
            ((frontMask & TAPE_F_L_BIT) == 0u) &&
            ((frontMask & TAPE_F_M_BIT) == 0u));
}

static inline bool TapeFeature_RightLockCandidate(uint8_t frontMask,
                                                  uint8_t rearMask)
{
    return (((frontMask & TAPE_F_RM_BIT) != 0u) &&
            ((rearMask & TAPE_R_RM_BIT) != 0u) &&
            ((frontMask & TAPE_F_R_BIT) == 0u) &&
            ((frontMask & TAPE_F_M_BIT) == 0u));
}

static inline void TapeFeature_UpdateCandidate(bool candidate,
                                               uint32_t nowMs,
                                               bool *active,
                                               uint32_t *sinceMs)
{
    if (candidate) {
        if (*active == false) {
            *active = true;
            *sinceMs = nowMs;
        }
    } else {
        *active = false;
        *sinceMs = nowMs;
    }
}

static inline bool TapeFeature_Debounced(bool candidate,
                                         bool active,
                                         uint32_t sinceMs,
                                         uint32_t nowMs)
{
    return (candidate &&
            active &&
            ((uint32_t)(nowMs - sinceMs) >= EDGE_LOCK_DEBOUNCE_MS));
}

static inline bool TapeFeature_UpdateLost(bool locked,
                                          bool everLocked,
                                          uint32_t nowMs,
                                          bool *lostActive,
                                          uint32_t *lostSinceMs)
{
    if (locked) {
        *lostActive = false;
        *lostSinceMs = nowMs;
        return false;
    }

    if (everLocked) {
        if (*lostActive == false) {
            *lostActive = true;
            *lostSinceMs = nowMs;
        }
        return ((uint32_t)(nowMs - *lostSinceMs) <= EDGE_LOST_GRACE_MS);
    }

    return false;
}

static inline void TapeFeature_Update(TapeFeatureEstimator_t *estimator,
                                      uint8_t frontMask,
                                      uint8_t rearMask,
                                      uint32_t nowMs,
                                      TapeFeature_t *feature)
{
    bool leftCandidate = TapeFeature_LeftLockCandidate(frontMask, rearMask);
    bool rightCandidate = TapeFeature_RightLockCandidate(frontMask, rearMask);

    TapeFeature_UpdateCandidate(leftCandidate, nowMs,
                                &estimator->leftCandidateActive,
                                &estimator->leftCandidateSince);
    TapeFeature_UpdateCandidate(rightCandidate, nowMs,
                                &estimator->rightCandidateActive,
                                &estimator->rightCandidateSince);

    feature->frontMask = frontMask;
    feature->rearMask = rearMask;
    feature->leftBoundaryDanger = ((frontMask & TAPE_F_L_BIT) != 0u);
    feature->rightBoundaryDanger = ((frontMask & TAPE_F_R_BIT) != 0u);
    feature->frontWideBlack =
        (TapeFeature_CountBits(frontMask) >= FRONT_WIDE_MIN_COUNT);
    feature->frontAllBlack =
        ((frontMask & (TAPE_F_L_BIT | TAPE_F_LM_BIT | TAPE_F_M_BIT |
                       TAPE_F_RM_BIT | TAPE_F_R_BIT)) ==
         (TAPE_F_L_BIT | TAPE_F_LM_BIT | TAPE_F_M_BIT |
          TAPE_F_RM_BIT | TAPE_F_R_BIT));
    feature->rearAnyBlack = ((rearMask & (TAPE_R_LM_BIT | TAPE_R_RM_BIT)) != 0u);
    feature->rearWideBlack =
        ((rearMask & (TAPE_R_LM_BIT | TAPE_R_RM_BIT)) ==
         (TAPE_R_LM_BIT | TAPE_R_RM_BIT));

    feature->leftEdgeLocked =
        TapeFeature_Debounced(leftCandidate,
                              estimator->leftCandidateActive,
                              estimator->leftCandidateSince,
                              nowMs);
    feature->rightEdgeLocked =
        TapeFeature_Debounced(rightCandidate,
                              estimator->rightCandidateActive,
                              estimator->rightCandidateSince,
                              nowMs);

    if (feature->leftEdgeLocked) {
        estimator->leftEverLocked = true;
    }
    if (feature->rightEdgeLocked) {
        estimator->rightEverLocked = true;
    }

    feature->leftEdgeSoftLost =
        TapeFeature_UpdateLost(feature->leftEdgeLocked,
                               estimator->leftEverLocked,
                               nowMs,
                               &estimator->leftLostActive,
                               &estimator->leftLostSince);
    feature->rightEdgeSoftLost =
        TapeFeature_UpdateLost(feature->rightEdgeLocked,
                               estimator->rightEverLocked,
                               nowMs,
                               &estimator->rightLostActive,
                               &estimator->rightLostSince);

    feature->confidence = 0;
    if (feature->leftEdgeLocked || feature->rightEdgeLocked) {
        feature->confidence += 3;
    }
    if (feature->leftEdgeSoftLost || feature->rightEdgeSoftLost) {
        feature->confidence += 1;
    }
    if (feature->frontWideBlack) {
        feature->confidence += 1;
    }
    if (feature->leftBoundaryDanger || feature->rightBoundaryDanger) {
        feature->confidence -= 2;
    }

    feature->lateralError = 0;
    if (feature->leftBoundaryDanger) {
        feature->lateralError += 2;
    } else if (feature->leftEdgeSoftLost && !feature->leftEdgeLocked) {
        feature->lateralError -= 1;
    }
    if (feature->rightBoundaryDanger) {
        feature->lateralError -= 2;
    } else if (feature->rightEdgeSoftLost && !feature->rightEdgeLocked) {
        feature->lateralError += 1;
    }

    feature->headingError = 0;
    if (((frontMask & TAPE_F_LM_BIT) != 0u) &&
        ((rearMask & TAPE_R_LM_BIT) == 0u)) {
        feature->headingError -= 1;
    }
    if (((frontMask & TAPE_F_RM_BIT) != 0u) &&
        ((rearMask & TAPE_R_RM_BIT) == 0u)) {
        feature->headingError += 1;
    }
}

#endif
