// Copyright 2020-2022 The Electric Coin Company
// Copyright 2022 The Halo2 developers
// Use of this source code is governed by a MIT/Apache-2.0 style license that
// can be found in the LICENSE-MIT.halo2 and the LICENCE-APACHE.halo2
// file.

#ifndef TACHYON_ZK_PLONK_CIRCUIT_CIRCUIT_H_
#define TACHYON_ZK_PLONK_CIRCUIT_CIRCUIT_H_

#include <memory>

#include "tachyon/zk/plonk/circuit/layouter.h"

namespace tachyon::zk {

template <typename Config>
class Circuit {
 public:
  using Field = typename Config::Field;

  virtual ~Circuit() = default;

  virtual std::unique_ptr<Circuit> WithoutWitness() const = 0;

  virtual Error Synthesize(const Config& config, Layouter<Field>* layouter) {
    return Error::kNone;
  }
};

}  // namespace tachyon::zk

#endif  // TACHYON_ZK_PLONK_CIRCUIT_CIRCUIT_H_
