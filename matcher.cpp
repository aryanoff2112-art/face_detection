#include "matcher.h"

#include "config.h"

using namespace cv;
using namespace std;

MatchResult identify(const Ptr<FaceRecognizerSF>& recognizer,
                      const Mat& liveEmbedding,
                      const vector<KnownFace>& db)
{
    MatchResult best{ "Unknown", -1.0, false };

    for (const KnownFace& kf : db){
        double score = recognizer->match(liveEmbedding, kf.embedding,
                                          FaceRecognizerSF::FR_COSINE);
        if (score > best.score){
            best.score = score;
            best.name = kf.name;
        }
    }

    if (best.score > config::COSINE_THRESHOLD)
        best.recognized = true;
    else
        best.name = "Unknown";

    return best;
}