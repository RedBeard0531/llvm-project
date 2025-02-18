// RUN: %clang_cc1 %s -verify -fopenacc

template<unsigned I, typename T>
void templ() {
#pragma acc loop collapse(I)
  for(int i = 0; i < 5;++i)
    for(int j = 0; j < 5; ++j)
      for(int k = 0; k < 5; ++k)
        for(int l = 0; l < 5; ++l)
          for(int m = 0; m < 5; ++m)
            for(int n = 0; n < 5; ++n)
              for(int o = 0; o < 5; ++o);

#pragma acc loop collapse(T::value)
  for(int i = 0;i < 5;++i)
    for(int j = 0; j < 5; ++j)
      for(int k = 0; k < 5; ++k)
        for(int l = 0; l < 5; ++l)
          for(int m = 0; m < 5;++m)
            for(;;)
              for(;;);

#pragma acc parallel vector_length(T::value)
  for(;;){}

#pragma acc parallel vector_length(I)
  for(;;){}

#pragma acc parallel async(T::value)
  for(;;){}

#pragma acc parallel async(I)
  for(;;){}

#pragma acc parallel async
  for(;;){}

  // expected-warning@+1{{OpenACC clause 'delete' not yet implemented, clause ignored}}
#pragma acc exit data delete(I)
  ;
}

struct S {
  static constexpr unsigned value = 5;
};

void use() {
  templ<7, S>();
}

namespace NS {
void NSFunc();

class RecordTy { // #RecTy
  static constexpr bool Value = false; // #VAL
  void priv_mem_function(); // #PrivMemFun
  public:
  static constexpr bool ValuePub = true;
  void mem_function();
};
template<typename T>
class TemplTy{};
void function();
}


  // expected-warning@+2{{OpenACC clause 'bind' not yet implemented, clause ignored}}
  // expected-warning@+1{{OpenACC construct 'routine' not yet implemented, pragma ignored}}
#pragma acc routine(use) bind(NS::NSFunc)
  // expected-error@+3{{'RecordTy' does not refer to a value}}
  // expected-note@#RecTy{{declared here}}
  // expected-warning@+1{{OpenACC construct 'routine' not yet implemented, pragma ignored}}
#pragma acc routine(use) bind(NS::RecordTy)
  // expected-error@+4{{'Value' is a private member of 'NS::RecordTy'}}
  // expected-note@#VAL{{implicitly declared private here}}
  // expected-warning@+2{{OpenACC clause 'bind' not yet implemented, clause ignored}}
  // expected-warning@+1{{OpenACC construct 'routine' not yet implemented, pragma ignored}}
#pragma acc routine(use) bind(NS::RecordTy::Value)
  // expected-warning@+2{{OpenACC clause 'bind' not yet implemented, clause ignored}}
  // expected-warning@+1{{OpenACC construct 'routine' not yet implemented, pragma ignored}}
#pragma acc routine(use) bind(NS::RecordTy::ValuePub)
  // expected-warning@+2{{OpenACC clause 'bind' not yet implemented, clause ignored}}
  // expected-warning@+1{{OpenACC construct 'routine' not yet implemented, pragma ignored}}
#pragma acc routine(use) bind(NS::TemplTy<int>)
  // expected-error@+2{{no member named 'unknown' in namespace 'NS'}}
  // expected-warning@+1{{OpenACC construct 'routine' not yet implemented, pragma ignored}}
#pragma acc routine(use) bind(NS::unknown<int>)
  // expected-warning@+2{{OpenACC clause 'bind' not yet implemented, clause ignored}}
  // expected-warning@+1{{OpenACC construct 'routine' not yet implemented, pragma ignored}}
#pragma acc routine(use) bind(NS::function)
  // expected-error@+4{{'priv_mem_function' is a private member of 'NS::RecordTy'}}
  // expected-note@#PrivMemFun{{implicitly declared private here}}
  // expected-warning@+2{{OpenACC clause 'bind' not yet implemented, clause ignored}}
  // expected-warning@+1{{OpenACC construct 'routine' not yet implemented, pragma ignored}}
#pragma acc routine(use) bind(NS::RecordTy::priv_mem_function)
  // expected-warning@+2{{OpenACC clause 'bind' not yet implemented, clause ignored}}
  // expected-warning@+1{{OpenACC construct 'routine' not yet implemented, pragma ignored}}
#pragma acc routine(use) bind(NS::RecordTy::mem_function)

  // expected-error@+2{{string literal with user-defined suffix cannot be used here}}
  // expected-warning@+1{{OpenACC construct 'routine' not yet implemented, pragma ignored}}
#pragma acc routine(use) bind("unknown udl"_UDL)

  // expected-warning@+3{{encoding prefix 'u' on an unevaluated string literal has no effect}}
  // expected-warning@+2{{OpenACC clause 'bind' not yet implemented, clause ignored}}
  // expected-warning@+1{{OpenACC construct 'routine' not yet implemented, pragma ignored}}
#pragma acc routine(use) bind(u"16 bits")
  // expected-warning@+3{{encoding prefix 'U' on an unevaluated string literal has no effect}}
  // expected-warning@+2{{OpenACC clause 'bind' not yet implemented, clause ignored}}
  // expected-warning@+1{{OpenACC construct 'routine' not yet implemented, pragma ignored}}
#pragma acc routine(use) bind(U"32 bits")
