interface Algorithm {
    name?: string;
}

interface KeyAlgorithm {
    name?: string;
}

interface CryptoKey {
    algorithm: KeyAlgorithm;
    extractable: boolean;
    type: string;
    usages: string[];
}

class SubtleCrypto {
    decrypt(algorithm: string | Algorithm, key: CryptoKey, data: ArrayBufferView): any{
        
    }
    deriveBits(algorithm: string | Algorithm, baseKey: CryptoKey, length: number): any{
        
    }
    deriveKey(algorithm: string | Algorithm, baseKey: CryptoKey, derivedKeyType: string | Algorithm, extractable: boolean, keyUsages: string[]): any{
        
    }
    digest(algorithm: string | Algorithm, data: ArrayBufferView): any{
        
    }
    encrypt(algorithm: string | Algorithm, key: CryptoKey, data: ArrayBufferView): any{
        
    }
    exportKey(format: string, key: CryptoKey): any{
        
    }
    generateKey(algorithm: string | Algorithm, extractable: boolean, keyUsages: string[]): any{
        
    }
    importKey(format: string, keyData: ArrayBufferView, algorithm: string | Algorithm, extractable: boolean, keyUsages: string[]): any{
        
    }
    sign(algorithm: string | Algorithm, key: CryptoKey, data: ArrayBufferView): any{
        
    }
    unwrapKey(format: string, wrappedKey: ArrayBufferView, unwrappingKey: CryptoKey, unwrapAlgorithm: string | Algorithm, unwrappedKeyAlgorithm: string | Algorithm, extractable: boolean, keyUsages: string[]): any{
        
    }
    verify(algorithm: string | Algorithm, key: CryptoKey, signature: ArrayBufferView, data: ArrayBufferView): any{
        
    }
    wrapKey(format: string, key: CryptoKey, wrappingKey: CryptoKey, wrapAlgorithm: string | Algorithm): any{
        
    }
}

class Crypto{
    subtle=new SubtleCrypto();
    getRandomValues(array: ArrayBufferView): ArrayBufferView{
        return null;
    }
}