/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/
#pragma once
#pragma warning(disable:4996)
#include <aws/gamelift/common/GameLift_EXPORTS.h>

#ifndef GAMELIFT_USE_STD
    #ifndef MAX_CERT_PATH_LENGTH
        #define MAX_CERT_PATH_LENGTH 1024
    #endif
    #ifndef MAX_HOST_NAME_LENGTH
        #define MAX_HOST_NAME_LENGTH 1024
    #endif
#endif

namespace Aws
{
namespace GameLift
{
namespace Server
{
namespace Model
{
  /**
   * <p>Represents the returned data in response to a request action.</p>
   */
  class AWS_GAMELIFT_API GetInstanceCertificateResult
  {
#ifdef GAMELIFT_USE_STD
  public:
      GetInstanceCertificateResult() {}

      /**
       * <p>Destructor.</p>
       */
      ~GetInstanceCertificateResult() {}

      /**
       * <p>Copy Constructor.</p>
       */
      GetInstanceCertificateResult(const GetInstanceCertificateResult& other) :
          m_certificate_path(other.m_certificate_path),
          m_certificate_chain_path(other.m_certificate_chain_path),
          m_private_key_path(other.m_private_key_path),
          m_hostname(other.m_hostname)
      { }

      /**
      * <p>Move Constructor.</p>
      */
      GetInstanceCertificateResult(GetInstanceCertificateResult&& other) 
      {
          *this = std::move(other);
      }

      /**
      * <p>Copy assignment Constructor.</p>
      */
      GetInstanceCertificateResult& operator= (const GetInstanceCertificateResult& other) 
      {
          m_certificate_path = other.m_certificate_path;
          m_certificate_chain_path = other.m_certificate_chain_path;
          m_private_key_path = other.m_private_key_path;
          m_hostname = other.m_hostname;

          return *this;
      }

      /**
       * <p>Move assignment Constructor.</p>
       */
      GetInstanceCertificateResult& operator= (GetInstanceCertificateResult&& other) 
      {
          m_certificate_path = std::move(other.m_certificate_path);
          m_certificate_chain_path = std::move(other.m_certificate_chain_path);
          m_private_key_path = std::move(other.m_private_key_path);
          m_hostname = std::move(other.m_hostname);

          return *this;
      }

    /**
     * <p>Certificate PEM file path.</p>
     */
    inline const std::string& GetCertificatePath() const
    { 
        return m_certificate_path;
    }

    /**
     * <p>Certificate PEM file path.</p>
     */
    inline void SetCertificatePath(const std::string& value) 
    { 
        m_certificate_path = value; 
    }

    /**
     * <p>Certificate PEM file path.</p>
     */
    inline void SetCertificatePath(std::string&& value) 
    {
        m_certificate_path = std::move(value);
    }

    /**
     * <p>Certificate PEM file path.</p>
     */
    inline void SetCertificatePath(const char* value)
    {
        m_certificate_path.assign(value); 
    }

    /**
     * <p>Certificate PEM file path.</p>
     */
    inline GetInstanceCertificateResult& WithCertificatePath(const std::string value)
    {
        SetCertificatePath(value);
        return *this;
    }

    /**
     * <p>Certificate PEM file path.</p>
     */
    inline GetInstanceCertificateResult& WithCertificatePath(const std::string&& value)
    {
        SetCertificatePath(value);
        return *this;
    }

    /**
     * <p>Certificate PEM file path.</p>
     */
    inline GetInstanceCertificateResult& WithCertificatePath(const char* value)
    {
        SetCertificatePath(value);
        return *this; 
    }

    /**
     * <p>PrivateKey PEM file path</p>
     */
    inline const std::string& GetPrivateKeyPath() const
    { 
        return m_private_key_path; 
    }

    /**
    * <p>PrivateKey PEM file path</p>
    */
    inline void SetPrivateKeyPath(const std::string& value)
    { 
        m_private_key_path = value; 
    }

    /**
    * <p>PrivateKey PEM file path</p>
    */
    inline void SetPrivateKeyPath(std::string&& value)
    { 
        m_private_key_path = std::move(value); 
    }

    /**
    * <p>PrivateKey PEM file path</p>
    */
    inline void SetPrivateKeyPath(const char* value)
    { 
        m_private_key_path.assign(value);
    }

    /**
    * <p>PrivateKey PEM file path</p>
    */
    inline GetInstanceCertificateResult& WithPrivateKeyPath(const std::string& value)
    { 
        SetPrivateKeyPath(value);
        return *this;
    }

    /**
    * <p>PrivateKey PEM file path</p>
    */
    inline GetInstanceCertificateResult& WithPrivateKeyPath(std::string&& value)
    { 
        SetPrivateKeyPath(value);
        return *this;
    }

    /**
    * <p>PrivateKey PEM file path</p>
    */
    inline GetInstanceCertificateResult& WithPrivateKeyPath(const char* value)
    { 
        SetPrivateKeyPath(value);
        return *this;
    }

    /**
    * <p>Certificate chain PEM file path.</p>
    */
    inline const std::string& GetCertificateChainPath() const
    {
        return m_certificate_chain_path;
    }

    /**
    * <p>Certificate chain PEM file path.</p>
    */
    inline void SetCertificateChainPath(const std::string& value)
    {
        m_certificate_chain_path = value;
    }

    /**
    * <p>Certificate chain PEM file path.</p>
    */
    inline void SetCertificateChainPath(std::string&& value)
    {
        m_certificate_chain_path = std::move(value);
    }

    /**
    * <p>Certificate chain PEM file path.</p>
    */
    inline void SetCertificateChainPath(const char* value)
    {
        m_certificate_chain_path.assign(value);
    }

    /**
    * <p>Certificate chain PEM file path.</p>
    */
    inline GetInstanceCertificateResult& WithCertificateChainPath(const std::string value)
    {
        SetCertificateChainPath(value);
        return *this;
    }

    /**
    * <p>Certificate chain PEM file path.</p>
    */
    inline GetInstanceCertificateResult& WithCertificateChainPath(const std::string&& value)
    {
        SetCertificateChainPath(value);
        return *this;
    }

    /**
    * <p>Certificate chain PEM file path.</p>
    */
    inline GetInstanceCertificateResult& WithCertificateChainPath(const char* value)
    {
        SetCertificateChainPath(value);
        return *this;
    }

    /**
    * <p>Hostname of this instance</p>
    */
    inline const std::string& GetHostName() const
    {
        return m_hostname;
    }

    /**
    * <p>Hostname of this instance</p>
    */
    inline void SetHostName(const std::string& value)
    {
        m_hostname = value;
    }

    /**
    * <p>Hostname of this instance</p>
    */
    inline void SetHostName(std::string&& value)
    {
        m_hostname = std::move(value);
    }

    /**
    * <p>Hostname of this instance</p>
    */
    inline void SetHostName(const char* value)
    {
        m_hostname.assign(value);
    }

    /**
    * <p>Hostname of this instance</p>
    */
    inline GetInstanceCertificateResult& WithHostName(const std::string value)
    {
        SetHostName(value);
        return *this;
    }

    /**
    * <p>Hostname of this instance</p>
    */
    inline GetInstanceCertificateResult& WithHostName(const std::string&& value)
    {
        SetHostName(value);
        return *this;
    }

    /**
    * <p>Hostname of this instance</p>
    */
    inline GetInstanceCertificateResult& WithHostName(const char* value)
    {
        SetHostName(value);
        return *this;
    }

  private:
    std::string m_certificate_path;
    std::string m_certificate_chain_path;
    std::string m_private_key_path;
    std::string m_hostname;
#else
public:
    GetInstanceCertificateResult()
    {
        memset(m_certificate_path, 0, sizeof(m_certificate_path));
        memset(m_certificate_chain_path, 0, sizeof(m_certificate_chain_path));
        memset(m_private_key_path, 0, sizeof(m_private_key_path));
        memset(m_hostname, 0, sizeof(m_hostname));
    }

    /**
    * <p>Destructor.</p>
    */
    ~GetInstanceCertificateResult() {}

    /**
    * <p>Copy Constructor.</p>
    */
    GetInstanceCertificateResult(const GetInstanceCertificateResult& other)
    {
        strncpy(m_certificate_path, other.m_certificate_path, sizeof(other.m_certificate_path));
        strncpy(m_certificate_chain_path, other.m_certificate_chain_path, sizeof(other.m_certificate_chain_path));
        strncpy(m_private_key_path, other.m_private_key_path, sizeof(other.m_private_key_path));
        strncpy(m_hostname, other.m_hostname, sizeof(other.m_hostname));
    }

    /**
    * <p>Move Constructor.</p>
    */
    GetInstanceCertificateResult(GetInstanceCertificateResult&& other)
    {
        *this = std::move(other);
    }

    /**
    * <p>Copy assignment Constructor.</p>
    */
    GetInstanceCertificateResult& operator= (const GetInstanceCertificateResult& other) 
    {
        strncpy(m_certificate_path, other.m_certificate_path, sizeof(other.m_certificate_path));
        strncpy(m_certificate_chain_path, other.m_certificate_chain_path, sizeof(other.m_certificate_chain_path));
        strncpy(m_private_key_path, other.m_private_key_path, sizeof(other.m_private_key_path));
        strncpy(m_hostname, other.m_hostname, sizeof(other.m_hostname));

        return *this;
    }

    /**
    * <p>Move assignment Constructor.</p>
    */
    GetInstanceCertificateResult& operator= (GetInstanceCertificateResult&& other) 
    {
        strncpy(m_certificate_path, other.m_certificate_path, sizeof(other.m_certificate_path));
        strncpy(m_certificate_chain_path, other.m_certificate_chain_path, sizeof(other.m_certificate_chain_path));
        strncpy(m_private_key_path, other.m_private_key_path, sizeof(other.m_private_key_path));
        strncpy(m_hostname, other.m_hostname, sizeof(other.m_hostname));

        memset(other.m_certificate_path, 0, sizeof(other.m_certificate_path));
        memset(other.m_certificate_chain_path, 0, sizeof(other.m_certificate_chain_path));
        memset(other.m_private_key_path, 0, sizeof(other.m_private_key_path));
        memset(other.m_hostname, 0, sizeof(other.m_hostname));

        return *this;
    }

    /**
     * <p>Certificate PEM file path.</p>
     */
    inline const char* GetCertificatePath() const
    { 
        return m_certificate_path;
    }

    /**
     * <p>Certificate PEM file path.</p>
     */
    inline void SetCertificatePath(const char* value)
    { 
        strncpy(m_certificate_path, value, sizeof(m_certificate_path));
        m_certificate_path[sizeof(m_certificate_path)-1] = '\0';
    }

    /**
     * <p>Certificate PEM file path.</p>
     */
    inline GetInstanceCertificateResult& WithCertificatePath(const char* value)
    { 
        SetCertificatePath(value);
        return *this;
    }

    /**
    * <p>Certificate chain PEM file path.</p>
    */
    inline const char* GetCertificateChainPath() const
    { 
        return m_certificate_chain_path;
    }

    /**
    * <p>Certificate chain PEM file path.</p>
    */
    inline void SetCertificateChainPath(const char* value)
    { 
        strncpy(m_certificate_chain_path, value, sizeof(m_certificate_chain_path));
        m_certificate_chain_path[sizeof(m_certificate_chain_path) - 1] = '\0';
    }

    /**
    * <p>Certificate chain PEM file path.</p>
    */
    inline GetInstanceCertificateResult& WithCertificateChainPath(const char* value)
    { 
        SetCertificateChainPath(value);
        return *this; 
    }

    /**
    * <p>Private key PEM file path.</p>
    */
    inline const char* GetPrivateKeyPath() const 
    { 
        return m_private_key_path;
    }

    /**
    * <p>Private key PEM file path.</p>
    */
    inline void SetPrivateKeyPath(const char* value)
    { 
        strncpy(m_private_key_path, value, sizeof(m_private_key_path));
        m_private_key_path[sizeof(m_private_key_path) - 1] = '\0';
    }

    /**
    * <p>Private key PEM file path.</p>
    */
    inline GetInstanceCertificateResult& WithPrivateKeyPath(const char* value)
    { 
        SetPrivateKeyPath(value);
        return *this; 
    }

    /**
    * <p>Hostname of this instance.</p>
    */
    inline const char* GetHostName() const
    {
        return m_hostname;
    }

    /**
    * <p>Hostname of this instance.</p>
    */
    inline void SetHostName(const char* value)
    {
        strncpy(m_hostname, value, sizeof(m_hostname));
        m_hostname[sizeof(m_hostname) - 1] = '\0';
    }

    /**
    * <p>Hostname of this instance.</p>
    */
    inline GetInstanceCertificateResult& WithHostName(const char* value)
    {
        SetHostName(value);
        return *this;
    }

  private:
    char m_certificate_path[MAX_CERT_PATH_LENGTH];
    char m_certificate_chain_path[MAX_CERT_PATH_LENGTH];
    char m_private_key_path[MAX_CERT_PATH_LENGTH];
    char m_hostname[MAX_HOST_NAME_LENGTH];
#endif
  };

} // namespace Model
} // namespace Server
} // namespace GameLift
} // namespace Aws
