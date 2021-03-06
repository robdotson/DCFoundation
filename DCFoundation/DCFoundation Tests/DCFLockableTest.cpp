//
//  DCFLockTests.cpp
//  DCFoundation
//
//  Created by Rob Dotson on 6/3/15.
//  Copyright (c) 2015 Design Complex LLC. All rights reserved.
//

#include "DCFTesting.h"
#include <DCFoundation/DCFObject.h>
#include <DCFoundation/DCFLockable.h>

#if 0

DCF_TEST_NAMESPACE_BEGIN

class DCFLockableTest : public ::testing::Test {};

using namespace DCF;
class MutexObj : public virtual DCFObject, public virtual DCFLockable<DCFMutex> {
    FRIEND_TEST( DCFLockableTest, mutexCopy );
    FRIEND_TEST( DCFLockableTest, mutexAssign );
    FRIEND_TEST( DCFLockableTest, mutexMove );
    
    virtual MutexObj * init() { this->DCFObject::init(); return this; }
    
    virtual const DCFHashCode hash() const {
        return DCFObject::hash();
    }
}; // MutexObj

TEST_F( DCFLockableTest, mutexCopy ) {
    MutexObj m;
    
    ASSERT_NE( &m, nullptr ) << "Couldn't create MutexObj";
    
    ASSERT_NE( &m.theLock(), nullptr ) << "Couldn't get lock handle";
    
    ASSERT_NO_THROW( MutexObj n( m ) ) << "Unable to copy construct lockable object!";
    
        // Lock the mutex
    DCFMutexGuard g( m.theLock() );
    
        // Same thread
    ASSERT_THROW( MutexObj n(m), std::runtime_error ) << "Shouldn't be able to copy a locked object!";
    
        // New thread
    std::thread t( [&m]() {
        ASSERT_THROW( MutexObj n(m), std::runtime_error ) << "Shouldn't be able to copy a locked object!";
    } );
    t.join();
}

TEST_F( DCFLockableTest, mutexAssign ) {
    MutexObj m;
    
    ASSERT_NE( &m, nullptr ) << "Couldn't create MutexObj";
    
    ASSERT_NE( &m.theLock(), nullptr ) << "Couldn't get lock handle";
    
    ASSERT_NO_THROW( MutexObj n = m ) << "Unable to assign lockable object!";
    
    DCFMutexGuard g( m.theLock() );
    ASSERT_THROW( MutexObj n = m, std::runtime_error ) << "Shouldn't be able to assign a locked object!";
    
    std::thread t( [&m]() {
        ASSERT_THROW( MutexObj n = m, std::runtime_error ) << "Shouldn't be able to assign a locked object!";
    } );
    t.join();
}

TEST_F( DCFLockableTest, mutexMove ) {
    MutexObj m;
    
    ASSERT_NE( &m, nullptr ) << "Couldn't create MutexObj";
    
    ASSERT_NE( &m.theLock(), nullptr ) << "Couldn't get lock handle";
    
    ASSERT_NO_THROW( MutexObj n( std::move( m ) ) ) << "Unable to move construct lockable object!";
    
    ASSERT_NO_THROW( MutexObj n = std::move( m ) ) << "Unable to move-assign lockable object";
    
    DCFMutexGuard g( m.theLock() );
    ASSERT_THROW( MutexObj n( std::move( m ) ), std::runtime_error ) << "Shouldn't be able to move construct a locked object!";
    ASSERT_THROW( MutexObj n = std::move( m ), std::runtime_error ) << "Shouldn't be able to move assign a locked object!";
    
    std::thread t( [&m]() {
        ASSERT_THROW( MutexObj n( std::move( m ) ), std::runtime_error ) << "Shouldn't be able to move construct a locked object!";
        ASSERT_THROW( MutexObj n = std::move( m ), std::runtime_error ) << "Shouldn't be able to move assign a locked object!";
    } );
    t.join();
}

class RecursiveMutexObj : public virtual DCFObject, public virtual DCFLockable<DCFRecursiveMutex> {
    FRIEND_TEST( DCFLockableTest, recursiveMutexCopy );
    FRIEND_TEST( DCFLockableTest, recursiveMutexAssign );
    FRIEND_TEST( DCFLockableTest, recursiveMutexMove );
    
    virtual const DCFHashCode hash() const {
        return DCFObject::hash();
    }
}; // RecursiveMutexObj

TEST_F( DCFLockableTest, recursiveMutexCopy ) {
    RecursiveMutexObj m;
    
    ASSERT_NE( &m, nullptr ) << "Couldn't create MutexObj";
    
    ASSERT_NE( &m.theLock(), nullptr ) << "Couldn't get lock handle";
    
    ASSERT_NO_THROW( RecursiveMutexObj n( m ) ) << "Unable to copy construct lockable object!";
    
        // Lock the mutex
    DCFRecursiveMutexGuard g( m.theLock() );
    
        // Same thread
    ASSERT_NO_THROW( RecursiveMutexObj n(m) ) << "Should be able to copy recursively locked object!";
    
        // New thread
    std::thread t( [&m]() {
        ASSERT_THROW( RecursiveMutexObj n(m), std::runtime_error ) << "Shouldn't be able to copy a recursively locked object from another thread!";
    } );
    t.join();
}

TEST_F( DCFLockableTest, recursiveMutexAssign ) {
    RecursiveMutexObj m;
    
    ASSERT_NE( &m, nullptr ) << "Couldn't create MutexObj";
    
    ASSERT_NE( &m.theLock(), nullptr ) << "Couldn't get lock handle";
    
    ASSERT_NO_THROW( RecursiveMutexObj n = m ) << "Unable to assign lockable object!";
    
    DCFRecursiveMutexGuard g( m.theLock() );
    ASSERT_NO_THROW( RecursiveMutexObj n = m ) << "Should be able to assign a recursively locked object!";
    
    std::thread t( [&m]() {
        ASSERT_THROW( RecursiveMutexObj n = m, std::runtime_error ) << "Shouldn't be able to assign a recursively locked object from another thread!";
    } );
    t.join();
}

TEST_F( DCFLockableTest, recursiveMutexMove ) {
    RecursiveMutexObj m;
    
    ASSERT_NE( &m, nullptr ) << "Couldn't create MutexObj";
    
    ASSERT_NE( &m.theLock(), nullptr ) << "Couldn't get lock handle";
    
    ASSERT_NO_THROW( RecursiveMutexObj n( std::move( m ) ) ) << "Unable to move construct lockable object!";
    
    ASSERT_NO_THROW( RecursiveMutexObj n = std::move( m ) ) << "Unable to move-assign lockable object";
    
    DCFRecursiveMutexGuard g( m.theLock() );
    ASSERT_NO_THROW( RecursiveMutexObj n( std::move( m ) ) ) << "Should be able to move construct a recursively locked object!";
    ASSERT_NO_THROW( RecursiveMutexObj n = std::move( m ) ) << "Should be able to move assign a recursively locked object!";
    
    std::thread t( [&m]() {
        ASSERT_THROW( RecursiveMutexObj n( std::move( m ) ), std::runtime_error ) << "Shouldn't be able to move construct a recursively locked object from another thread!";
        ASSERT_THROW( RecursiveMutexObj n = std::move( m ), std::runtime_error ) << "Shouldn't be able to move assign a recursively locked object from another thread!";
    } );
    t.join();
}

DCF_TEST_NAMESPACE_END
#endif // 0
